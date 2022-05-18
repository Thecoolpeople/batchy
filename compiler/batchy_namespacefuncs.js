let lookupFuncs = {
	"gpio.digital.mode": function(arg){if(arg.length != 2){
							alert("gpio.digital.mode needs 2 arguments: PIN,VALUE")
							return []
						}
	let a = [2,1].concat(intTo4(arg[0].value))
	a = a.concat([2,2]).concat(intTo4(arg[1].value))
	let c = [9,0,1,0,0,0]
	return [].concat(a,c)
	},
	"gpio.digital.write": function(arg){if(arg.length != 2){
							alert("gpio.digital.write needs 2 arguments: PIN,VALUE")
							return []
						}
	let a = [2,1].concat(intTo4(arg[0].value))
	a = a.concat([2,2]).concat(intTo4(arg[1].value))
	let c = [9,0,2,0,0,0]
	return [].concat(a,c)
	},
	"gpio.digital.read": function(arg){if(arg.length != 1){
							alert("gpio.digital.read needs 1 arguments: PIN")
							return []
						}
	return []
	},
	"gpio.analog.mode": function(arg){if(arg.length != 2){
							alert("gpio.analog.mode needs 2 arguments: PIN,VALUE")
							return []
						}
	return []
	},
	"gpio.analog.write": function(arg){if(arg.length != 2){
							alert("gpio.analog.write needs 2 arguments: PIN,VALUE")
							return []
						}
	return []
	},
	"gpio.analog.read": function(arg){if(arg.length != 1){
							alert("gpio.analog.read needs 1 arguments: PIN")
							return []
						}
	return []
	},
	"delay.ms": function(arg){if(arg.length != 1){
							alert("delay.ms needs 1 arguments: Duration ms")
							return []
						}
	let a = [2,1].concat(intTo4(arg[0].value))
	let c = [9,0,10,0,0,0]
	return [].concat(a,c)
	},
	"delay.us": function(arg){if(arg.length != 1){
							alert("delay.us needs 1 arguments: Duration us")
							return []
						}
	let a = [2,1].concat(intTo4(arg[0].value))
	let c = [9,0,11,0,0,0]
	return [].concat(a,c)
	},
	"serial.init": function(arg){if(arg.length != 1){
							alert("serial.init needs 1 arguments: speed")
							return []
						}
	let a = [2,1].concat(intTo4(arg[0].value))
	let c = [9,0,12,0,0,0]
	return [].concat(a,c)
	},
	"serial.readAllRegister": function(arg){if(arg.length != 0){
							alert("serial.readAllRegister needs 0 arguments: ")
							return []
						}
	let c = [9,0,13,0,0,0]
	return c
	},
	"serial.writeAllRegister": function(arg){if(arg.length != 0){
							alert("serial.writeAllRegister needs 0 arguments: ")
							return []
						}
	let c = [9,0,14,0,0,0]
	return c
	}
	}