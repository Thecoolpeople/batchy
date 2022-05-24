let lookupFuncs = {
	"gpio_digital_mode": function(arg){if(arg.length != 2){
	alert("gpio_digital_mode needs 2 arguments: PIN,VALUE")
	return []
	}
	let a = [BATCHYCMDID.REG_SET_SINGLE,1].concat(intTo4(arg[0]))
	a = a.concat([BATCHYCMDID.REG_SET_SINGLE,2]).concat(intTo4(arg[1]))
	let c = [BATCHYCMDID.BATCHY_CMD,0,1,0,0,0]
	return [].concat(a,c)
	},
	"gpio_digital_write": function(arg){if(arg.length != 2){
	alert("gpio_digital_write needs 2 arguments: PIN,VALUE")
	return []
	}
	let a = [BATCHYCMDID.REG_SET_SINGLE,1].concat(intTo4(arg[0]))
	a = a.concat([BATCHYCMDID.REG_SET_SINGLE,2]).concat(intTo4(arg[1]))
	let c = [BATCHYCMDID.BATCHY_CMD,0,2,0,0,0]
	return [].concat(a,c)
	},
	"gpio_digital_read": function(arg){if(arg.length != 1){
	alert("gpio_digital_read needs 1 arguments: PIN")
	return []
	}
	let a = [BATCHYCMDID.REG_SET_SINGLE,1].concat(intTo4(arg[0]))
	let c = [BATCHYCMDID.BATCHY_CMD,0,3,0,0,0]
	return [].concat(a,c)
	},
	"gpio.analog.mode": function(arg){if(arg.length != 2){
	alert("gpio.analog.mode needs 2 arguments: PIN,VALUE")
	return []
	}
	let a = [BATCHYCMDID.REG_SET_SINGLE,1].concat(intTo4(arg[0]))
	a = a.concat([BATCHYCMDID.REG_SET_SINGLE,2]).concat(intTo4(arg[1]))
	let c = [BATCHYCMDID.BATCHY_CMD,0,4,0,0,0]
	return [].concat(a,c)
	},
	"gpio.analog.write": function(arg){if(arg.length != 2){
	alert("gpio.analog.write needs 2 arguments: PIN,VALUE")
	return []
	}
	let a = [BATCHYCMDID.REG_SET_SINGLE,1].concat(intTo4(arg[0]))
	a = a.concat([BATCHYCMDID.REG_SET_SINGLE,2]).concat(intTo4(arg[1]))
	let c = [BATCHYCMDID.BATCHY_CMD,0,5,0,0,0]
	return [].concat(a,c)
	},
	"gpio.analog.read": function(arg){if(arg.length != 1){
	alert("gpio.analog.read needs 1 arguments: PIN")
	return []
	}
	return []
	},
	"delay_ms": function(arg){if(arg.length != 1){
	alert("delay_ms needs 1 arguments: Duration ms")
	return []
	}
	let a = [BATCHYCMDID.REG_SET_SINGLE,1].concat(intTo4(arg[0]))
	let c = [BATCHYCMDID.BATCHY_CMD,0,10,0,0,0]
	return [].concat(a,c)
	},
	"delay_us": function(arg){if(arg.length != 1){
	alert("delay_us needs 1 arguments: Duration us")
	return []
	}
	let a = [BATCHYCMDID.REG_SET_SINGLE,1].concat(intTo4(arg[0]))
	let c = [BATCHYCMDID.BATCHY_CMD,0,11,0,0,0]
	return [].concat(a,c)
	},
	"serial.init": function(arg){if(arg.length != 1){
	alert("serial.init needs 1 arguments: speed")
	return []
	}
	let a = [BATCHYCMDID.REG_SET_SINGLE,1].concat(intTo4(arg[0]))
	let c = [BATCHYCMDID.BATCHY_CMD,0,12,0,0,0]
	return [].concat(a,c)
	},
	"serial.readAllRegister": function(arg){if(arg.length != 0){
	alert("serial.readAllRegister needs 0 arguments: ")
	return []
	}
	let c = [BATCHYCMDID.BATCHY_CMD,0,13,0,0,0]
	return c
	},
	"serial.writeAllRegister": function(arg){if(arg.length != 0){
	alert("serial.writeAllRegister needs 0 arguments: ")
	return []
	}
	let c = [BATCHYCMDID.BATCHY_CMD,0,14,0,0,0]
	return c
	}
	}