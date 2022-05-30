let lookupFuncs = {
	"gpio_digital_mode": function(arglen){if(arglen != 2){
	alert("gpio_digital_mode needs 2 arguments: PIN,VALUE")
	return []
	}
	return [BATCHYCMDID.BATCHY_CMD,0,1,0,0,0]
	},
	"gpio_digital_write": function(arglen){if(arglen != 2){
	alert("gpio_digital_write needs 2 arguments: PIN,VALUE")
	return []
	}
	return [BATCHYCMDID.BATCHY_CMD,0,2,0,0,0]
	},
	"gpio_digital_read": function(arglen){if(arglen != 1){
	alert("gpio_digital_read needs 1 arguments: PIN")
	return []
	}
	return [BATCHYCMDID.BATCHY_CMD,0,3,0,0,0]
	},
	"gpio_analog_mode": function(arglen){if(arglen != 2){
	alert("gpio_analog_mode needs 2 arguments: PIN,VALUE")
	return []
	}
	return [BATCHYCMDID.BATCHY_CMD,0,4,0,0,0]
	},
	"gpio_analog_write": function(arglen){if(arglen != 2){
	alert("gpio_analog_write needs 2 arguments: PIN,VALUE")
	return []
	}
	return [BATCHYCMDID.BATCHY_CMD,0,5,0,0,0]
	},
	"gpio_analog_read": function(arglen){if(arglen != 1){
	alert("gpio_analog_read needs 1 arguments: PIN")
	return []
	}
	return []
	},
	"delay_ms": function(arglen){if(arglen != 1){
	alert("delay_ms needs 1 arguments: Duration ms")
	return []
	}
	return [BATCHYCMDID.BATCHY_CMD,0,10,0,0,0]
	},
	"delay_us": function(arglen){if(arglen != 1){
	alert("delay_us needs 1 arguments: Duration us")
	return []
	}
	return [BATCHYCMDID.BATCHY_CMD,0,11,0,0,0]
	},
	"serial_init": function(arglen){if(arglen != 1){
	alert("serial_init needs 1 arguments: speed")
	return []
	}
	return [BATCHYCMDID.BATCHY_CMD,0,12,0,0,0]
	},
	"serial_readAllRegister": function(arglen){if(arglen != 0){
	alert("serial_readAllRegister needs 0 arguments: ")
	return []
	}
	return [BATCHYCMDID.BATCHY_CMD,0,13,0,0,0]
	},
	"serial_writeAllRegister": function(arglen){if(arglen != 0){
	alert("serial_writeAllRegister needs 0 arguments: ")
	return []
	}
	return [BATCHYCMDID.BATCHY_CMD,0,14,0,0,0]
	}
}