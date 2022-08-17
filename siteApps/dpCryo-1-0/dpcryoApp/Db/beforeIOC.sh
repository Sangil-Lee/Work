nidaqmxconfig --help
Usage:
   nidaqmxconfig --import <filename> [--replace | --eraseconfig [--preserveabsentdevices] ]
   nidaqmxconfig --export <filename>
   nidaqmxconfig --self-test <device>
   nidaqmxconfig --reset <device>
   nidaqmxconfig --find-net-devs [--timeout <sec>] [--verbose | -v]
   nidaqmxconfig --add-net-dev <address> [--reserve] [--timeout <sec>] [--name <device>]
   nidaqmxconfig --del-net-dev <device>
   nidaqmxconfig --reserve <device> [--force]
   nidaqmxconfig --unreserve <device>
   nidaqmxconfig [--help]

1.$>nidaqmxconfig --add-net-dev "192.168.1.101"
Success
Added: cDAQ9185-1FF3DAD

2.$>nilsdev 
cDAQ9185-1FF3DAD

3.$>nidaqmxconfig --reserve cDAQ9185-1FF3DAD
Success

4.$>nilsdev 
cDAQ9185-1FF3DAD
   cDAQ9185-1FF3DADMod2
   cDAQ9185-1FF3DADMod3
