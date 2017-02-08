pva 2 pva gateway/proxy


To build


```shell
git clone --recursive -b pva2pva https://github.com/mdavidsaver/v4workspace
cd v4workspace
make -C epics-base
make -c pvData
make -c pvAccess
make -c pvaSrv
make -C pva2pva
```

Host setup for testing

```shell
tunctl -t tap0
tunctl -t tap1
ifconfig tap0 10.5.1.1/24 up
ifconfig tap1 10.5.2.1/24 up
```

In the following demo the gateway server side (GWS) listens on 10.5.1.1/24
while the gateway client side (GWC) searches on 10.5.2.1/24.

To run the demo open three shells in parallel.

One

```shell
cd pvaSrv
cat << EOF > test.db
record(calc, "$(N)") {
  field(SCAN, "1 second")
  field(INPA, "$(N) NPP")
  field(CALC, "A+1")
}
EOF
cat << EOF > test.cmd
epicsEnvSet("EPICS_PVAS_INTF_ADDR_LIST","10.5.2.1")
epicsEnvSet("EPICS_PVA_ADDR_LIST", "10.5.2.255")
epicsEnvSet("EPICS_PVA_AUTO_ADDR_LIST","NO")
dbLoadRecords("test.db", "N=cnt:a")
iocInit()
startPVAServer
EOF
./bin/linux-x86/softIocPVA test.cmd
```

Two

```shell
cd pva2pva
cat << EOF > test.cmd
epicsEnvSet("EPICS_PVAS_INTF_ADDR_LIST","10.5.1.1")
epicsEnvSet("EPICS_PVA_ADDR_LIST", "10.5.2.255")
epicsEnvSet("EPICS_PVA_AUTO_ADDR_LIST","NO")
gwstart()
EOF
./bin/linux-x86/p2p test.cmd
```

Three

```shell
cd pvAccess
export EPICS_PVA_ADDR_LIST=10.5.1.1
export EPICS_PVA_AUTO_ADDR_LIST=NO
./bin/linux-x86_64/pvget -m cnt:a
```
