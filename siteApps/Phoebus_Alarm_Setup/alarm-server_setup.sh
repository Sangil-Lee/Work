#!/bin/bash

###### Before Following Procedure ###############
###$>cp -rf phoebus/app/alarm/example ~/alarm
###$>cp -rf phoebus/service/alarm-server ~/
###$>cd alarm

export hostip=192.168.75.71

wget https://dlcdn.apache.org/kafka/3.3.1/kafka_2.12-3.3.1.tgz
tar -vzxf kafka_2.12-3.3.1.tgz 
sleep 1
ln -s kafka_2.12-3.3.1 kafka

sed -i 's/\/tmp\/zookeeper/\/home\/ctrluser\/zookeeper-logs/g' kafka/config/zookeeper.properties
sed -i 's/\/tmp\/kafka-logs/\/home\/ctrluser\/kafka-logs/g' kafka/config/server.properties
sed -i 's/timeout.ms=18000/timeout.ms=300000/g' kafka/config/server.properties

sed -i 's/#listeners=PLAINTEXT:\/\/:9092/listeners=PLAINTEXT:\/\/192.168.75.71:9092/g' kafka/config/server.properties
sed -i'' -r -e '/#advertised.listeners=/i\advertised.host.name=devDesk' kafka/config/server.properties
sed -i'' -r -e '/#advertised.listeners=/a\auto.create.topics.enable=false' kafka/config/server.properties
sed -i 's/#advertised.listeners=PLAINTEXT:\/\/your.host.name:9092/advertised.listeners=PLAINTEXT:\/\/192.168.75.71:9092/g' kafka/config/server.properties


##[zookeeper.service]
#Environment=JAVA_HOME=/PATH/TO/JDK
sed -i 's/JAVA_HOME=\/PATH\/TO\/JDK/JAVA_HOME=\/usr\/lib\/jvm\/java-17-openjdk-amd64/g' zookeeper.service

#ExecStart=/opt/kafka/bin/zookeeper-server-start.sh /opt/kafka/config/zookeeper.properties
sed -i 's/\/opt\/kafka\/bin\/zookeeper-server-start.sh \/opt\/kafka\/config\/zookeeper.properties/\/home\/ctrluser\/alarm\/kafka\/bin\/zookeeper-server-start.sh \/home\/ctrluser\/alarm\/kafka\/config\/zookeeper.properties/g' zookeeper.service

#ExecStop=/opt/kafka/bin/zookeeper-server-stop.sh
sed -i 's/\/opt\/kafka\/bin\/zookeeper-server-stop.sh/\/home\/ctrluser\/alarm\/kafka\/bin\/zookeeper-server-stop.sh/g' zookeeper.service

#User=DESIRED_USER
#Group=DESIRED_GROUP
sed -i 's/DESIRED_USER/ctrluser/g' zookeeper.service
sed -i 's/DESIRED_GROUP/ctrluser/g' zookeeper.service

##[kafka.service]
#User=DESIRED_USER
#Group=DESIRED_GROUP
sed -i 's/DESIRED_USER/ctrluser/g' kafka.service
sed -i 's/DESIRED_GROUP/ctrluser/g' kafka.service

#Environment=JAVA_HOME=/PATH/TO/JDK
sed -i 's/JAVA_HOME=\/PATH\/TO\/JDK/JAVA_HOME=\/usr\/lib\/jvm\/java-17-openjdk-amd64/g' kafka.service

#ExecStart=/opt/kafka/bin/kafka-server-start.sh /opt/kafka/config/server.properties
sed -i 's/\/opt\/kafka\/bin\/kafka-server-start.sh \/opt\/kafka\/config\/server.properties/\/home\/ctrluser\/alarm\/kafka\/bin\/kafka-server-start.sh \/home\/ctrluser\/alarm\/kafka\/config\/server.properties/g' kafka.service

#ExecStop=/opt/kafka/bin/kafka-server-stop.sh
sed -i 's/\/opt\/kafka\/bin\/kafka-server-stop.sh/\/home\/ctrluser\/alarm\/kafka\/bin\/kafka-server-stop.sh/g' kafka.service

sudo cp kafka.service zookeeper.service /etc/systemd/system


sed -i "s#localhost#$hostip#g" create_alarm_topics.sh
sed -i "s#localhost#$hostip#g" delete_alarm_topics.sh

echo "Next, create topic(Accelerator,Demo,...) by using create_alarm_topic.sh 
     Then, should be started phoebus/services/alarm-server/alarm-server.sh"

echo "Generate demo.db for test "
cat > demo.db <<EOF
record(ai, "DEMO:PV1")
{
   field(PINI, "YES")
   field(VAL,  0)
   field(PREC, 3)
   field(HOPR, 100)
   field(LOPR, -100.0)
   field(HIHI, 10.0)
   field(HIGH, 5.0)
   field(LOW, -5.0)
   field(LOLO, -10)
   field(HHSV, "MAJOR")
   field(HSV, "MINOR")
   field(LSV, "MINOR")
   field(LLSV, "MAJOR")
}
record(ai, "DEMO:PV2")
{
   field(PINI, "YES")
   field(VAL,  500)
   field(PREC, 3)
   field(HOPR, 100)
   field(LOPR, -100.0)
   field(HIHI, 10.0)
   field(HIGH, 5.0)
   field(LOW, -5.0)
   field(LOLO, -10)
   field(HHSV, "MAJOR")
   field(HSV, "MINOR")
   field(LSV, "MINOR")
   field(LLSV, "MAJOR")
}
record(ai, "DEMO:PV3")
{
   field(PINI, "YES")
   field(VAL,  0)
   field(PREC, 3)
   field(HOPR, 100)
   field(LOPR, -100.0)
   field(HIHI, 10.0)
   field(HIGH, 5.0)
   field(LOW, -5.0)
   field(LOLO, -10)
   field(HHSV, "MAJOR")
   field(HSV, "MINOR")
   field(LSV, "MINOR")
   field(LLSV, "MAJOR")
}
record(ai, "DEMO:PV4")
{
   field(PINI, "YES")
   field(VAL,  0)
   field(PREC, 3)
   field(HOPR, 100)
   field(LOPR, -100.0)
   field(HIHI, 10.0)
   field(HIGH, 5.0)
   field(LOW, -5.0)
   field(LOLO, -10)
   field(HHSV, "MAJOR")
   field(HSV, "MINOR")
   field(LSV, "MINOR")
   field(LLSV, "MAJOR")
}
record(ai, "DEMO:PV5")
{
   field(PINI, "YES")
   field(VAL,  0)
   field(PREC, 3)
   field(HOPR, 100)
   field(LOPR, -100.0)
   field(HIHI, 10.0)
   field(HIGH, 5.0)
   field(LOW, -5.0)
   field(LOLO, -10)
   field(HHSV, "MAJOR")
   field(HSV, "MINOR")
   field(LSV, "MINOR")
   field(LLSV, "MAJOR")
}
record(ai, "DEMO:PV6")
{
   field(PINI, "YES")
   field(VAL,  0)
   field(PREC, 3)
   field(HOPR, 100)
   field(LOPR, -100.0)
   field(HIHI, 10.0)
   field(HIGH, 5.0)
   field(LOW, -5.0)
   field(LOLO, -10)
   field(HHSV, "MAJOR")
   field(HSV, "MINOR")
   field(LSV, "MINOR")
   field(LLSV, "MAJOR")
}
record(ai, "DEMO:PV7")
{
   field(PINI, "YES")
   field(VAL,  0)
   field(PREC, 3)
   field(HOPR, 100)
   field(LOPR, -100.0)
   field(HIHI, 10.0)
   field(HIGH, 5.0)
   field(LOW, -5.0)
   field(LOLO, -10)
   field(HHSV, "MAJOR")
   field(HSV, "MINOR")
   field(LSV, "MINOR")
   field(LLSV, "MAJOR")
}


### TEST for Demo topic
record(ai, "TEST:PV1")
{
   field(PINI, "YES")
   field(VAL,  0)
   field(PREC, 3)
   field(HOPR, 100)
   field(LOPR, -100.0)
   field(HIHI, 10.0)
   field(HIGH, 5.0)
   field(LOW, -5.0)
   field(LOLO, -10)
   field(HHSV, "MAJOR")
   field(HSV, "MINOR")
   field(LSV, "MINOR")
   field(LLSV, "MAJOR")
}
record(ai, "TEST:PV2")
{
   field(PINI, "YES")
   field(VAL,  500)
   field(PREC, 3)
   field(HOPR, 100)
   field(LOPR, -100.0)
   field(HIHI, 10.0)
   field(HIGH, 5.0)
   field(LOW, -5.0)
   field(LOLO, -10)
   field(HHSV, "MAJOR")
   field(HSV, "MINOR")
   field(LSV, "MINOR")
   field(LLSV, "MAJOR")
}
record(ai, "TEST:PV3")
{
   field(PINI, "YES")
   field(VAL,  0)
   field(PREC, 3)
   field(HOPR, 100)
   field(LOPR, -100.0)
   field(HIHI, 10.0)
   field(HIGH, 5.0)
   field(LOW, -5.0)
   field(LOLO, -10)
   field(HHSV, "MAJOR")
   field(HSV, "MINOR")
   field(LSV, "MINOR")
   field(LLSV, "MAJOR")
}
record(ai, "TEST:PV4")
{
   field(PINI, "YES")
   field(VAL,  0)
   field(PREC, 3)
   field(HOPR, 100)
   field(LOPR, -100.0)
   field(HIHI, 10.0)
   field(HIGH, 5.0)
   field(LOW, -5.0)
   field(LOLO, -10)
   field(HHSV, "MAJOR")
   field(HSV, "MINOR")
   field(LSV, "MINOR")
   field(LLSV, "MAJOR")
}
record(ai, "TEST:PV5")
{
   field(PINI, "YES")
   field(VAL,  0)
   field(PREC, 3)
   field(HOPR, 100)
   field(LOPR, -100.0)
   field(HIHI, 10.0)
   field(HIGH, 5.0)
   field(LOW, -5.0)
   field(LOLO, -10)
   field(HHSV, "MAJOR")
   field(HSV, "MINOR")
   field(LSV, "MINOR")
   field(LLSV, "MAJOR")
}
record(ai, "TEST:PV6")
{
   field(PINI, "YES")
   field(VAL,  0)
   field(PREC, 3)
   field(HOPR, 100)
   field(LOPR, -100.0)
   field(HIHI, 10.0)
   field(HIGH, 5.0)
   field(LOW, -5.0)
   field(LOLO, -10)
   field(HHSV, "MAJOR")
   field(HSV, "MINOR")
   field(LSV, "MINOR")
   field(LLSV, "MAJOR")
}
record(ai, "TEST:PV7")
{
   field(PINI, "YES")
   field(VAL,  0)
   field(PREC, 3)
   field(HOPR, 100)
   field(LOPR, -100.0)
   field(HIHI, 10.0)
   field(HIGH, 5.0)
   field(LOW, -5.0)
   field(LOLO, -10)
   field(HHSV, "MAJOR")
   field(HSV, "MINOR")
   field(LSV, "MINOR")
   field(LLSV, "MAJOR")
}
EOF
cat > ~/alarm-server/demo.xml <<EOF
<?xml version='1.0' encoding='utf8'?>
<config name="Accelerator">
    <component name="DEMO:GROUP1">
        <pv name="DEMO:PV1">
            <enabled>true</enabled>
            <filter>DEMO:PV2 > 10</filter>
        </pv>
        <pv name="DEMO:PV2">
            <enabled>true</enabled>
        </pv>
        <pv name="DEMO:PV3">
            <enabled>true</enabled>
        </pv>
    </component>
    <component name="DEMO:GROUP2">
        <component name="DEMO:GROUP3">
            <pv name="DEMO:PV4">
                <enabled>true</enabled>
            </pv>
            <pv name="DEMO:PV5">
                <enabled>true</enabled>
            </pv>
        </component>
        <pv name="DEMO:PV6">
            <enabled>true</enabled>
        </pv>
        <pv name="DEMO:PV7">
            <enabled>true</enabled>
        </pv>
    </component>
</config>
EOF
cat > ~/alarm-server/test.xml <<EOF
<?xml version='1.0' encoding='utf8'?>
<config name="Demo">
    <component name="TEST:GROUP1">
        <pv name="TEST:PV1">
            <enabled>true</enabled>
            <filter>TEST:PV2 > 10</filter>
        </pv>
        <pv name="TEST:PV2">
            <enabled>true</enabled>
        </pv>
        <pv name="TEST:PV3">
            <enabled>true</enabled>
        </pv>
    </component>
    <component name="TEST:GROUP2">
        <component name="TEST:GROUP3">
            <pv name="TEST:PV4">
                <enabled>true</enabled>
            </pv>
            <pv name="TEST:PV5">
                <enabled>true</enabled>
            </pv>
        </component>
        <pv name="TEST:PV6">
            <enabled>true</enabled>
        </pv>
        <pv name="TEST:PV7">
            <enabled>true</enabled>
        </pv>
    </component>
</config>
EOF

###### Phoebus Alarm Test Procedure ###############
### $>sudo systemctl start zookeeper.service
### $>sudo systemctl start kafka.service
### $>cd alarm
### $alarm>./create_alarm_topics.sh Accelerator
### $alarm>./create_alarm_topics.sh Demo
### $alarm>tmux new -d -s IOC softIoc -d demo.db
### $alarm>cd ../alarm-server
### $alarm-server>./alarm-server.sh -server 192.168.75.71:9092 -config Accelerator -import demo.xml
### $alarm-server>./alarm-server.sh -server 192.168.75.71:9092 -config Demo -import test.xml

### $alarm-server>tmux new -s Alarm_Topics (and split pane, Ctrl+b, -)
### $alarm-server>./alarm-server.sh -server 192.168.75.71:9092 -config Accelerator (1pane)
### $alarm-server>./alarm-server.sh -server 192.168.75.71:9092 -config Demo (2pane)
### Then, alarm test
### $alarm-server>caput DEMO:PV4 15
### $alarm-server>caput TEST:PV4 15
###

###### Another Host: Phoebus (Alarm Panel, Alarm Tree, Alarm Table) ###############
##settings.ini
##org.phoebus.pv.ca/addr_list=192.168.75.70 192.168.75.71
##org.phoebus.pv.ca/auto_addr_list=true
##org.phoebus.applications.alarm/server=192.168.75.71:9092
##org.phoebus.applications.alarm/config_name=Demo
##org.phoebus.applications.alarm/config_names=Accelerator, Demo
##org.phoebus.applications.alarm/alarm_area_level=2

## restart phoebus and check Alarm Panel, Alarm Tree, Alarm Table


##[alarm-server.service]
#sed -i 's/DESIRED_USER/ctrluser/g' alarm_server.service
#sed -i 's/DESIRED_GROUP/ctrluser/g' alarm_server.service
#Environment=JAVA_HOME=/PATH/TO/JDK
#sed -i 's/JAVA_HOME=\/PATH\/TO\/JDK/JAVA_HOME=\/usr\/lib\/jvm\/java-17-openjdk-amd64/g' alarm_server.service
#Environment=SERVER=/PATH/TO/phoebus-alarm-server
#sed -i 's/SERVER=\/PATH\/TO\/phoebus-alarm-server=\/home\/ctrluser\/phoebus\/services\/alarm-server/g' alarm_server.service
#Environment=PATH=/PATH/TO/JDK/bin:/usr/bin
#sed -i 's/PATH=\/PATH\/TO\/JDK\/bin/\/usr\/lib\/jvm\/java-17-openjdk-amd64\/bin/g' alarm_server.service
#ExecStart=/usr/bin/procServ --foreground --noautorestart --name alarm-server --chdir ${SERVER} --logfile ${SERVER}/console.log ${PORT} ./alarm-server.sh -settings /PATH/TO/phoebus.ini -config ${CONFIG}
