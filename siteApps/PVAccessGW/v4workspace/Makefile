all: real-all
distclean: base-distclean

real-all: base-all
base-distclean: real-distclean

real-%: RELEASE.local
	$(MAKE) -C pvData $*
	$(MAKE) -C pvAccess $*
	$(MAKE) -C pva2pva $*
	$(MAKE) -C pvaSrv $*
	$(MAKE) -C normativeTypes $*
	$(MAKE) -C pvaClient $*
	$(MAKE) -C pvaPy $*

base-%: RELEASE.local
	$(MAKE) -C epics-base $*

clean:
	rm -f RELEASE.local

RELEASE.local: RELEASE.local.in
	echo "THE_ROOT_DIR=$$PWD" > $@
	cat $< >> $@

.PHONY: all clean
