#!/usr/bin/env python3
"""
Run RTEMS test harness(es) with QEMU

  ./test-rtems-qemu.py \
   ./src/libCom/test/O.RTEMS-pc386/testspec \
   ./src/ioc/db/test/O.RTEMS-pc386/testspec \
   ./src/std/filters/test/O.RTEMS-pc386/testspec \
   ./src/std/rec/test/O.RTEMS-pc386/testspec

When planning to run tests, build Base with

make RTEMS_QEMU_FIXUPS=YES
"""

import logging
_log = logging.getLogger(__name__)

import os, sys, signal, re
from subprocess import PIPE, STDOUT, DEVNULL, TimeoutExpired, Popen
from shutil import copyfile, which
from tempfile import TemporaryDirectory

def getargs():
    import argparse
    P = argparse.ArgumentParser(description='Run testspec(s) for an RTEMS target')
    P.add_argument('specs', metavar='testspec', nargs='+')
    P.add_argument('--arch', metavar='ARCH', default='i386',
                   help='QEMU arch. name (default "i386")')
    P.add_argument('-t','--timeout', metavar='SECS', type=int, default=600,
                   help='test timeout interval')
    P.add_argument('-D','--display', action='store_true',
                   help='Show the VGA console')
    P.add_argument('-n','--dry-run', action='store_true',
                   help="Don't start qemu, just print what would be done")
    P.add_argument('-l','--level', metavar='NAME', default='INFO',
                   type=logging.getLevelName,
                   help="Python log level (default INFO)")

    return P.parse_args()

# Don't forget "-no-reboot" or the first test will spin until timeout
targetinfo = {
    'RTEMS-pc386':{
        'cmd':['qemu-system-i386', '-no-reboot', '-m', '128',
               '-net', 'nic,model=ne2k_pci',
               '-net', 'user,hostname=qemuhost,tftp=%(tftp)s',
               '-serial', 'stdio',
               '-kernel', '%(image)s',
               '-append', '--console=com1', # For pc386, RTEMS >=4.10
              ],
    }
}

class TapProc(object):
    def __init__(self):
        self.reset()
        self.totaltest, self.totalpass = 0, 0
        self.log = []
        self._P = [
            self._pass,
            self._fail,
            self._diag,
            self._skip,
            self._name,
            self._plan,
        ]
        
    def reset(self):
        self.name = ''
        self.expect = None
        self.ntest = 0
        self.npass = self.nfail = self.nskip = self.bonus = 0

    def sum(self):
        _log.info("Pass: %d Fail: %d Skip: %d Bonus: %d",
                  self.npass, self.nfail, self.nskip, self.bonus)

        T = self.npass + self.nfail + self.nskip + self.bonus
        ok = self.nfail==0
        if self.expect:
            _log.info("Tests %d/%d", self.ntest, self.expect)
            ok &= self.ntest==self.expect
        elif T>0:
            _log.info("Tests %d", self.ntest)
        if ok:
            self.totalpass += 1

    def final(self):
        if self.name:
            self.sum()
        _log.info("Total errors: %d", len(self.log))
        for N,L in self.log:
            _log.error(' %s: %s', N, L)
        if self.totalpass==self.totaltest:
            _log.info('Total Tests passing %d/%d', self.totalpass, self.totaltest)
        else:
            _log.error('Total Tests passing %d/%d', self.totalpass, self.totaltest)
        self.result = self.totalpass == self.totaltest

    def __call__(self, line):
        for P in self._P:
            M = P._re.match(line)
            if M:
                P(M)
                sys.stdout.write(line)
                sys.stdout.flush()
                return
        _log.debug("?: %s", line[:-1])

    def _name(self, M):
        if self.name:
            _log.info("Leave %s", self.name)
            self.sum()
        self.reset()
        self.name = M.group(1)
        _log.info("Enter %s", self.name)
        self.totaltest += 1
    # '***** testname *****'
    _name._re = re.compile(r'\*{5}\s+(\S+)\s+\*{5}')

    def _plan(self, M):
        A, B = map(int,M.groups())
        assert A==1, (A,B)
        self.expect = B
    # '1..29'
    _plan._re = re.compile(r'(\d+)\.\.(\d+)')

    def _pass(self, M):
        self.npass += 1
        self.ntest += 1
        if re.match(r'.*\s#\s+TODO\s.*', M.group(0)):
            self.bonus += 1

    # 'ok 42 - something'
    # 'ok 42 - something' # TODO why not'
    _pass._re = re.compile(r'ok\s+\d+\s+-\s.*')

    def _fail(self, M):
        if re.match(r'.*\s#\s+TODO\s.*', M.group(0)):
            self.npass += 1
            self.ntest += 1
        else:
            self.nfail += 1
            self.ntest += 1
            self.log.append((self.name, M.group(0)))

    # 'not ok 42 - something'
    # 'not ok 42 - something' # TODO why not'
    _fail._re = re.compile(r'not\s+ok\s+\d+\s+-\s.*')

    def _diag(self, M):
        pass
    # '# some text'
    _diag._re = re.compile(r'#.*')

    def _skip(self, M):
        self.npass += 1
        self.ntest += 1
        self.nskip += 1
    # 'ok 42 # SKIP why message
    _skip._re = re.compile(r'ok\s+\d+\s+#\s+SKIP\s.*')

def parsespec(spec):
    with open(spec, 'r') as FP:
        # Lines have the form
        #  Key: Value with spaces
        # Split on ':' then strip key and value
        # Return a dictionary of {'Key':'Value ...', ...}
        return dict([map(str.strip,L.split(':',1)) for L in FP.readlines()])

def runspec(specfile, proc, args):
    specdir = os.path.dirname(specfile)
    conf = parsespec(specfile)

    if 'Harness' not in conf or conf.get('OS-class','')!='RTEMS' or conf.get('Target-arch','') not in targetinfo:
        _log.warn('Skipping %s', specfile)
        return

    info = targetinfo[conf['Target-arch']]

    exe = which(info['cmd'][0])
    _log.debug('Emulator %s', exe)
    info['cmd'][0] = exe

    # Harness: dbTestHarness.boot; epicsRunDbTests
    harness = os.path.join(specdir, conf['Harness'].split(';')[0].strip())
    if harness.endswith('.boot'):
        harness = harness[:-5] # QEMU needs the ELF file
    _log.debug('Harness: %s', harness)

    if not os.path.isfile(harness):
        raise RuntimeError('Harness does not exist')

    with TemporaryDirectory() as D:
        params = {'tftp':D, 'image':harness}

        D = os.path.join(D, 'epics', 'qemuhost')
        os.makedirs(D)

        # Copy in TESTFILES
        for F in conf.get('Files','').split():
            src = os.path.join(specdir, F)
            dst = os.path.join(D, os.path.basename(F))
            _log.debug('Copy %s -> %s', src, dst)
            copyfile(src, dst)

        cmd = [I%params for I in info['cmd']]

        if not args.display:
            cmd += ['-display', 'none']

        _log.debug('Cmd: %s', cmd)

        if args.dry_run:
            print(' '.join(cmd))
        else:
            if args.timeout and args.timeout>0:
                def timo(*args):
                    _log.error('QEMU Timeout')
                    sys.exit(2)
                signal.signal(signal.SIGALRM, timo)
                signal.alarm(args.timeout)
                _log.debug("Start timeout of %f", args.timeout)

            P = Popen(cmd, stdin=DEVNULL, stderr=STDOUT, stdout=PIPE, universal_newlines=True)

            try:
                for L in P.stdout:
                    if not L:
                        break
                    proc(L)

                P.wait(10)
                _log.info('Done %s', specfile)
            except:
                if P.returncode is None:
                    try:
                        _log.warn('Force QEMU termination')
                        P.terminate()
                        P.wait(10)
                    except:
                        _log.error('Kill QEMU')
                        P.kill()
                raise

            if args.timeout>0:
                signal.alarm(0)
            return P.returncode

def main(args):
    T = TapProc()
    try:
        for S in args.specs:
            _log.info('Test %s', S)
            runspec(S, T, args)
    finally:
        T.final()

if __name__=='__main__':
    args = getargs()
    logging.basicConfig(level=args.level,
                        format='%(asctime)-15s %(levelname)s %(message)s')
    main(args)
