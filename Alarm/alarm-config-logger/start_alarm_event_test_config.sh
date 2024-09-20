THIS_SRC=${BASH_SOURCE[0]:-${0}}
if [ -L $THIS_SRC ]; then    
	SRC_PATH="$( cd -P "$( dirname $(readlink -f "$THIS_SRC") )" && pwd )"
else    
	SRC_PATH="$( cd -P "$( dirname "$THIS_SRC" )" && pwd )"
fi
sh $SRC_PATH/alarm-config-logger.sh -properties $SRC_PATH/alarm_event_test_config_logger.properties

