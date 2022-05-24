var ast

BATCHYCMDID = {
	REG_CLEAR_ALL: 0,
	REG_CLEAR_SINGLE: 1,
	REG_SET_SINGLE: 2,
	MATH_ADD: 3,
	MATH_SUB: 4,
	MATH_MUL: 5,
	MATH_DIV: 6,
	MATH_MOD: 7,
	STACK_PUSH: 10,
	STACK_POP: 11,
	STACK_PUT: 12,
	STACK_GET: 13,
	JUMP_NORMAL: 20,
	JUMP_JAL: 21,
	JUMP_JALRET: 22,
	BATCHY_CMD: 30,
	CONDITION: 31
}

//parse C to ast
function astGenerate(){
	ast = cparse(document.getElementById("Ccode").innerText);
	document.getElementById("Cast").innerText = JSON.stringify(ast, undefined, 2).replace(/ /g, "\u00a0");
}

//parse ast to bytecode
function bytecodeGenerate(){
	astGenerate()

	let convertCommand = function(functionName, cmd, stack, astNames){
		//Returns the number of height from the step in the current function for a variable
		function getVariableIDFromStack(stack, varname){
			let stackLength = stack.length
			for(let i=1; i<=stack.length; i++){
				let stackEntry = stack[stackLength - i]
				if(stackEntry.type == "FunctionCall"){
					alert("Variable inside Function not found")
					return 0
				}else if(stackEntry.type == "VariableDeclaration"){
					console.log(">>>>>", stackEntry.name, varname)
					if(stackEntry.name == varname)
						return i
				}
			}
		}
		function getParsed(functionName, cmd, stack, astNames){
			switch(cmd.type){
				case "ExpressionStatement":
					return getParsed(functionName, cmd.expression, stack, astNames)
				break;

				case "CallExpression":{
					let name = cmd.base.value
					let parameter = cmd.arguments
					if(parameter.length != 1 || parameter[0] != null)
						parameter = parameter.map((c,i)=>{console.log(c,i);return getParsed(functionName, c, stack, astNames)})

					console.log(parameter)
					//check if express is a function call for a user function
					if(astNames.includes(name)){
						bc = [BATCHYCMDID.JUMP_JAL, 0, name,0,0,0]	//This will be replaced in the main
						stack.push({type: "FunctionCall", name: name})
						//TODO parameter push to stack
					}else{
						//this is a function from the namespace
						bc = lookupFuncs[name](parameter)
						//console.log(expression, parameter, bc)
					}
					return {bc:bc, stack:stack}
				}break;

				case "VariableDeclaration":{
					let type = cmd.defType.name
					if(cmd.defType.name != "int")
						alter("only ints are supported!")
					
					let name = cmd.name

					let value = getParsed(functionName, cmd.value, stack, astNames)
					console.log(value, typeof value)
					if(typeof value == "number"){	//Numbervalue
						bc = [BATCHYCMDID.REG_SET_SINGLE, 1, ...intTo4(value),		//set register 1
							BATCHYCMDID.STACK_PUSH, 1, 0, 0, 0, 0]					//push to stack
					}else if(typeof value == "string"){	//String
						//check if value is a variable
						let height = getVariableIDFromStack(stack, value)
						bc = [BATCHYCMDID.STACK_GET, 1, ...intTo4(height),			//get from stack and write to register 1
							BATCHYCMDID.STACK_PUSH, 1, 0, 0, 0, 0]					//push to stack
					}
					stack.push({type: "VariableDeclaration", name: name, defType:type})
					return {bc:bc, stack:stack}
				}break;

				case "Literal":
					return cmd.value
				break;

				case "Identifier":
					if(cmd.value == "false") return 0
					if(cmd.value == "true") return 1
					return cmd.value
				break;

				
			}
		}

		//run parsing
		console.log(cmd)

		let ret = getParsed(functionName, cmd, stack, astNames)
		bc = ret.bc
		stack = ret.stack;


		/*let type = cmd.type			//get command type
		let bc = []
		switch(type){
			case "VariableDeclaration":
				let varName = cmd.name
				switch(cmd.defType.name){
					case "int":	//int has 4 bytes
						stack[stack.length] = {type:"variable", defType: "int", name:varName, function:functionName}
					break;
				}
				
				if(!cmd.value){
					cmd.value = {type:"Literal", value:0}	//set to zero if no value
				}

				let value = get_expression(cmd.value)
				if(typeof value == "number"){	//Numbervalue
					bc = [BATCHYCMDID.REG_SET_SINGLE, 1, ...intTo4(value),
						BATCHYCMDID.STACK_PUSH, 1, 0, 0, 0, 0]
				}else if(value in lookupFuncs){	//FunctionCall
					alert("TODO")
				}else{							//Other Variable
					//Search in stack for variable
				}
			break;
			case "ExpressionStatement":		//function call
				let expression = get_expression(cmd.expression)
				let arguments = get_arguments(cmd.expression)

				//check if express is a function call for a user function
				if(astNames.includes(expression)){
					bc = [BATCHYCMDID.JUMP_JAL, 0, expression,0,0,0]	//This will be replaced in the main
				}else{
					//this is a function from the namespace
					bc = lookupFuncs[expression](arguments)
					//console.log(expression, arguments, bc)
				}
			break;
			case "IfStatement":
				function Condition(condition){
					switch(condition.left.type){
						case "BinaryExpression":
							Condition(condition.left)
						break;
						case "Identifier":

						break;
					}

					
				}

			break;
		}*/

		return {bc: bc, stack: stack}
	}
		
	
	let bodyParse = function(name, body, stack, astNames){	//parse whole function
		let func_bc = []
		let stackFunc = []
		body.forEach((cmd)=>{
			let ret = convertCommand(name, cmd, stackFunc, astNames)	//parse single command
			let bc = ret.bc
			stackFunc = ret.stack
			func_bc = func_bc.concat(bc)
		})
		if(name != "main"){
			func_bc = func_bc.concat([BATCHYCMDID.JUMP_JALRET,0,0,0,0,0])	//This is jal return. see batchy.h
		}
		return func_bc
	}

	//all astNames
	astNames = ast.map(e=>{return e.name})
	
	//loop through all functions
	let full_bc = {}
	let stack  = []
	ast.forEach((func)=>{
		let name = func.name			//get function name
		let type = func.defType.name	//get function type
		console.log(">function", type, name)
		full_bc[name] = bodyParse(name, func.body, stack, astNames)
	})

	BYTECODE_FunctionsStart = {}
	BYTECODE = [BATCHYCMDID.JUMP_NORMAL, 0, "main", 0,0,0]	//start with main function
	for(bc_name in full_bc){
		BYTECODE_FunctionsStart[bc_name] = BYTECODE.length
		BYTECODE = BYTECODE.concat(full_bc[bc_name])
	}
	
	//replace all userFuncs
	for(let i=0; i<BYTECODE.length; i += 6){
		if(BYTECODE[i] == BATCHYCMDID.JUMP_NORMAL || BYTECODE[i] == BATCHYCMDID.JUMP_JAL){
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
