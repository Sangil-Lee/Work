# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "BITSLIDE_COUNTER_FORCE" -parent ${Page_0}
  ipgui::add_param $IPINST -name "COMMA_IS_LSB_FORCE" -parent ${Page_0}


}

proc update_PARAM_VALUE.BITSLIDE_COUNTER_FORCE { PARAM_VALUE.BITSLIDE_COUNTER_FORCE } {
	# Procedure called to update BITSLIDE_COUNTER_FORCE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.BITSLIDE_COUNTER_FORCE { PARAM_VALUE.BITSLIDE_COUNTER_FORCE } {
	# Procedure called to validate BITSLIDE_COUNTER_FORCE
	return true
}

proc update_PARAM_VALUE.COMMA_IS_LSB_FORCE { PARAM_VALUE.COMMA_IS_LSB_FORCE } {
	# Procedure called to update COMMA_IS_LSB_FORCE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.COMMA_IS_LSB_FORCE { PARAM_VALUE.COMMA_IS_LSB_FORCE } {
	# Procedure called to validate COMMA_IS_LSB_FORCE
	return true
}


proc update_MODELPARAM_VALUE.BITSLIDE_COUNTER_FORCE { MODELPARAM_VALUE.BITSLIDE_COUNTER_FORCE PARAM_VALUE.BITSLIDE_COUNTER_FORCE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.BITSLIDE_COUNTER_FORCE}] ${MODELPARAM_VALUE.BITSLIDE_COUNTER_FORCE}
}

proc update_MODELPARAM_VALUE.COMMA_IS_LSB_FORCE { MODELPARAM_VALUE.COMMA_IS_LSB_FORCE PARAM_VALUE.COMMA_IS_LSB_FORCE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.COMMA_IS_LSB_FORCE}] ${MODELPARAM_VALUE.COMMA_IS_LSB_FORCE}
}

