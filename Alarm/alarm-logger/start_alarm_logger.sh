THIS_SRC=${BASH_SOURCE[0]:-${0}}

if [ -L $THIS_SRC ]; then    
	SRC_PATH="$( cd -P "$( dirname $(readlink -f "$THIS_SRC") )" && pwd )"
else    
	SRC_PATH="$( cd -P "$( dirname "$THIS_SRC" )" && pwd )"
fi
sh $SRC_PATH/alarm-logger.sh -properties $SRC_PATH/alarm_logger.properties -kafka_properties $SRC_PATH/kafka_client.properties 

#./alarm-logger.sh -topics Accelerator,Demo -es_host localhost -es_port 9200 -es_sniff false -bootstrap.servers 192.168.75.70:9092
