#./alarm-server.sh -server 192.168.75.70:9092 -config Demo -kafka_properties demo_client.properties
#

THIS_SRC=${BASH_SOURCE[0]:-${0}}
if [ -L $THIS_SRC ]; then    
	SRC_PATH="$( cd -P "$( dirname $(readlink -f "$THIS_SRC") )" && pwd )"
else    
	SRC_PATH="$( cd -P "$( dirname "$THIS_SRC" )" && pwd )"
fi


# At first, comment remove follow line
sh ${SRC_PATH}/alarm-server.sh -server 192.168.75.210:9092 -config Demo -import ${SRC_PATH}/demo_config.xml
sh ${SRC_PATH}/alarm-server.sh -server 192.168.75.210:9092 -config Demo
#./alarm-server.sh -server 192.168.75.101:9092 -config Demo -settings settings.ini 
#
