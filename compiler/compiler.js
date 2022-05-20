var ast

//parse C to ast
function astGenerate(){
	ast = cparse(document.getElementById("Ccode").innerText);
	document.getElementById("Cast").innerText = JSON.stringify(ast, undefined, 2).replace(/ /g, "\u00a0");
}

//parse ast to bytecode
function bytecodeGenerate(){
	astGenerate()

	let convertCommand = function(cmd, astNames){
		let get_expression = function(ep){
			switch(ep.type){
				case "CallExpression":
					return ep.base.value
				break;
				case "Identifier":
					return ep.value
				break;
				case "BinaryExpression":
					return get_expression(ep.left)+"."+get_expression(ep.right)
				break;
			}
		}
		let get_arguments = function(ep){
			switch(ep.type){
				case "CallExpression":
					return ep.arguments
				break;
				case "BinaryExpression":
					return get_arguments(ep.right)
				break;
			}
		}


		//run parsing
		console.log(cmd)
		let type = cmd.type			//get command type
		let bc = []
		switch(type){
			case "VariableDeclaration":
				alert("variableDeclaration in work");
			break;
			case "ExpressionStatement":		//function call
				let expression = get_expression(cmd.expression)
				let arguments = get_arguments(cmd.expression)

				//check if express is a function call for a user function
				if(astNames.includes(expression)){
					bc = [11, 0, expression,0,0,0]	//This will be replaced in the main
				}else{
					//this is a function from the namespace
					bc = lookupFuncs[expression](arguments)
					//console.log(expression, arguments, bc)
				}
			break;
		}

		return bc
	}
		
	
	let bodyParse = function(name, body, astNames){	//parse whole function
		let func_bc = []
		body.forEach((cmd)=>{
			let bc = convertCommand(cmd, astNames)	//parse single command
			func_bc = func_bc.concat(bc)
		})
		if(name != "main"){
			func_bc = func_bc.concat([12,0,0,0,0,0])	//This is jal return. see batchy.h
		}
		return func_bc
	}

	//all astNames
	astNames = ast.map(e=>{return e.name})
	
	//loop through all functions
	let full_bc = {}
	ast.forEach((func)=>{
		let name = func.name			//get function name
		let type = func.defType.name	//get function type
		console.log(">function", type, name)
		full_bc[name] = bodyParse(name, func.body, astNames)
	})

	BYTECODE_FunctionsStart = {}
	BYTECODE = [10, 0, "main", 0,0,0]	//start with main function
	for(bc_name in full_bc){
		BYTECODE_FunctionsStart[bc_name] = BYTECODE.length
		BYTECODE = BYTECODE.concat(full_bc[bc_name])
	}
	
	//replace all userFuncs
	for(let i=0; i<BYTECODE.length; i += 6){
		if(BYTECODE[i] == 10 || BYTECODE[i] == 11){
			let startNumber = BYTECODE_FunctionsStart[BYTECODE[i+2]];
			let startNumberChar = intTo4(startNumber)
			BYTECODE[i+2] = startNumberChar[0]
			BYTECODE[i+3] = startNumberChar[1]
			BYTECODE[i+4] = startNumberChar[2]
			BYTECODE[i+5] = startNumberChar[3]
		}
	}

	document.getElementById("Cbytecode").innerHTML = ""
	let pad = function(num, size) {
		num = num.toString();
		let left = size - num.length
		for(let i=0; i < left; i++)
			num = "&nbsp;" + num;
		return num;
	}

	//display to webpage
	for(let i=0; i<BYTECODE.length; i++){
		document.getElementById("Cbytecode").innerHTML += pad(BYTECODE[i],3)+", "
		if((i+1) % 6 == 0)
			document.getElementById("Cbytecode").innerHTML += '<br>'
	}
}

let allFunctionsShow = function(){
	let f = ""
	for(k in lookupFuncs){
		f += k + '<br>'
	}
	document.getElementById("allFunctions").innerHTML = f
}

let intTo4 = function(value){
	return [value&0xFF, (value>>8)&0xFF, (value>>16)&0xFF, (value>>24)&0xFF]
}
