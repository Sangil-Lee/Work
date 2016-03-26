# How to Setup .bash_aliases into your environment

## Step 1 
  Select one of the following methods 

1.0 copy .bash_aliases in $HOME

1.1 in $HOME, make symbolic link to point .bash_aliases

    $ ln -s ~/scripts_for_epics/linux.conf/bash/.bash_aliases $HOME/.bash_aliases

## Step 2 

2.  Edit .bashrc to put the following lines 

```bash
   if [ -f ~/.bash_aliases ]; then
     . ~/.bash_aliases
   fi
```

## Step 3 

   Reopen the terminal or source .bashrc

# How to use the most interesting features 

## goApps 

The command *goApps* brings your position to $RAON_SITEAPPS

1. without argument, go $RAON_SITEAPPS

```bash
jhlee@kaffee:~$ goApps
jhlee@kaffee:~/epics/R3.14.12.5/siteApps$ 
```

2. with argument, go $RAON_SITEAPPS/argument

```bash
jhlee@kaffee:~$ goApps keithley6514
jhlee@kaffee:~/epics/R3.14.12.5/siteApps/keithley6514$ 
```
   Note that the argument must be the same name of a siteApps application.

3. If there is no EPICS variables, it will force to set the default one.

```bash
jhlee@kaffee:~$ goApps
No EPICS environment variables are found. Force to set the default one.
~/epics/R3.14.12.5/setEpicsEnv.sh

jhlee@kaffee:~/epics/R3.14.12.5/siteApps$ 
```


## goLibs 

It is the same purpose command for $RAON_SITELIBS.

```bash
jhlee@kaffee:~$ goLibs configure
jhlee@kaffee:~/epics/R3.14.12.5/siteLibs/configure$ 
```

## goEpics

It is the same command for $EPICS_PATH
```bash
jhlee@kaffee:~$ goEpics 
No EPICS environment variables are found. Force to set the default one.
~/epics/R3.14.12.5/setEpicsEnv.sh

jhlee@kaffee:~/epics/R3.14.12.5$ 

jhlee@kaffee:~$ goEpics base
jhlee@kaffee:~/epics/R3.14.12.5/base$ 
```


## goSynApps

It is the same command for $EPICS_SYNAPPS
```bash
jhlee@kaffee:~$ goSynApps 
No EPICS environment variables are found. Force to set the default one.
~/epics/R3.14.12.5/setEpicsEnv.sh

jhlee@kaffee:~$ goSynApps configure
jhlee@kaffee:~/epics/R3.14.12.5/epicsLibs/synApps_5_8/support/configure$ 

```





## goIOC 

Bring you to iocBoot/ioc$argument directory
```bash
jhlee@kaffee:~$ goIoc snmp2
jhlee@kaffee:~/epics/R3.14.12.5/siteApps/snmp2/iocBoot/iocsnmp2$ 
```

## runIoc 

Run IOC for $argument 

```bash
jhlee@kaffee:~$ runIoc snmp2
#!../../bin/linux-x86_64/snmp2
## You may have to change snmp2 to something else
## everywhere it appears in this file
< envPaths
epicsEnvSet("ARCH","linux-x86_64")
epicsEnvSet("IOC","iocsnmp2")
epicsEnvSet("TOP","/home/jhlee/epics/R3.14.12.5/siteApps/snmp2")
cd /home/jhlee/epics/R3.14.12.5/siteApps/snmp2
epicsEnvSet("MIBDIRS", "/home/jhlee/epics/R3.14.12.5/siteLibs/snmpLib/mibs")
## Register all support components
dbLoadDatabase "dbd/snmp2.dbd"
...........
```

Wednesday, April  8 09:52:19 KST 2015
