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
	MATH_EQ:  8,
	MATH_NEQ: 9,
	MATH_AND: 10,
	MATH_OR:  11,
	MATH_GREATER: 12,
	MATH_SMALLER: 13,
	MATH_GREATEREQ: 14,
	MATH_SMALLEREQ: 15,

	STACK_PUSH: 16,
	STACK_POP:  17,
	STACK_PUT:  18,
	STACK_GET:  19,

	JUMP_NORMAL: 20,
	JUMP_JAL: 	 21,
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

	//Returns the number of height from the step in the current function for a variable
	function getVariableIDFromStack(stack, varname){
		let stackLength = stack.length
		let nrIF = 0;
		for(let i=1; i<=stack.length; i++){
			let stackEntry = stack[stackLength - i]
			if(stackEntry.type == "FunctionCall" && !stackEntry.name.includes("|>>if<<|")){
				alert("Variable inside Function not found")
				return 0
			}else if(stackEntry.type == "FunctionCall" && stackEntry.name.includes("|>>if<<|")){	//count if
				nrIF++;
			}else if(stackEntry.type == "VariableDeclaration"){
				if(stackEntry.name == varname)
					return i - nrIF;
			}
		}
	}

	//validate Statement
	function validateStatement(value, register, stack){
		if(typeof value == "number"){
			bc = [BATCHYCMDID.REG_SET_SINGLE, register, ...intTo4(value)]
		}else if(typeof value == "string"){
			let height = getVariableIDFromStack(stack, value)
			if(height == 0)
				alert("Variable ("+value+") not found")
			bc = [BATCHYCMDID.STACK_GET, register, ...intTo4(height)]
		}else if(typeof value == "object"){
			bc = value.bc
			stack = value.stack;
		}

		return {bc:bc, stack:stack}
	}

	//clear cache to and incl last function call
	function deleteAllToLastFunctioncallFromStack(stack){
		let stackLength = stack.length
		let bc = []
		for(let i=1; i<=stackLength; i++){
			let stackEntry = stack[stackLength - i]
			if(stackEntry.type == "FunctionCall"){
				break;
			}
			stack.pop()
			bc = [...bc, BATCHYCMDID.STACK_POP, 0, 0, 0, 0, 0]
		}
		return {bc:bc, stack: stack}
	}

	let convertCommand = function(functionName, cmd, stack, astNames){
		function getParsed(functionName, cmd, stack, astNames, args){
			if(!args) args = {}
			
			let outputReg = args.outputReg || 0;
			let inputRegL = args.inputRegL || 1;
			let inputRegR = args.inputRegR || 2;

			switch(cmd.type){
				case "ExpressionStatement":
					return getParsed(functionName, cmd.expression, stack, astNames)
				break;

				case "IfStatement":{
					let bc = []
					let condition
					if(cmd.condition){
						condition = getParsed(functionName, cmd.condition, stack, {outputReg: outputReg, inputRegL: outputReg, inputRegR: outputReg+1})
					}else{
						condition = 0
					}

					//validate statement
					let ret = validateStatement(condition, outputReg, stack); bc = [...bc, ...ret.bc]; stack = ret.stack


					//body part
					stack.push({type: "FunctionCall", name: functionName+"|>>if<<|"})

					bc_body = []
					cmd.body.forEach((cmd)=>{
						let ret = convertCommand(functionName, cmd, stack, astNames)	//parse single command
						stack = ret.stack
						bc_body = bc_body.concat(ret.bc)
					})

					ret = deleteAllToLastFunctioncallFromStack(stack)
					stack = ret.stack
					bc_body = bc_body.concat(ret.bc)	//delete all local vars and co


					//else part
					stack.push({type: "FunctionCall", name: functionName+"|>>if<<|"})

					bc_else = []
					cmd.else.forEach((cmd)=>{
						let ret = convertCommand(functionName, cmd, stack, astNames)	//parse single command
						stack = ret.stack
						bc_else = bc_else.concat(ret.bc)
					})

					ret = deleteAllToLastFunctioncallFromStack(stack)
					stack = ret.stack
					bc_else = bc_else.concat(ret.bc)	//delete all local vars and co


					//if validation
					bc = [...bc,
						BATCHYCMDID.CONDITION, outputReg, ...intTo4(0),
						BATCHYCMDID.JUMP_NORMAL, 0, "+12",0,0,0,					//body part
						BATCHYCMDID.JUMP_NORMAL, 0, "+"+(12+bc_body.length),0,0,0,	//else part
						...bc_body,
						BATCHYCMDID.JUMP_NORMAL, 0, "+"+(6+bc_else.length),0,0,0,  //jump after the else part
						...bc_else,
					];


					return {bc:bc, stack:stack}
				}break;

				case "CallExpression":{
					let bc = []
					let name = cmd.base.value
					let parameter = cmd.arguments
					if(parameter.length != 1 || parameter[0] != null)
						parameter = parameter.map((c,i)=>{
							console.log(c,i)
							let parsed = getParsed(functionName, c, stack, astNames)
							let ret = validateStatement(parsed, i+1, stack)	//i+1: because we start with register 1 instead of 0
							stack = ret.stack;
							return ret.bc
						})
					else
						parameter = []

					//check if express is a function call for a user function
					if(astNames.includes(name)){
						bc = [BATCHYCMDID.JUMP_JAL, 0, name,0,0,0]	//This will be replaced in the main
						//TODO parameter push to stack
					}else{
						//this is a function from the namespace
						bc = parameter.flat()
						bc = [...bc, ...lookupFuncs[name](parameter.length)]
						console.log(bc)
						//console.log(expression, parameter, bc)
					}
					return {bc:bc, stack:stack}
				}break;

				case "PrefixExpression":{
					//TODO   this is called, for example when int a = -1;  because of the minus
				}break;

				case "BinaryExpression":{
					let bc = []

					//Set operator '='
					if(cmd.operator == "="){
						let right = getParsed(functionName, cmd.right, stack, astNames, {outputReg: inputRegR, inputRegL: inputRegR, inputRegR: inputRegR+1})
						//validate statement
						let ret = validateStatement(right, inputRegR, stack); bc = [...bc, ...ret.bc]; stack = ret.stack

						if(cmd.left.type == "Identifier"){
							let height = getVariableIDFromStack(stack, cmd.left.value)
							if(height == 0)
								alert("Variable ("+cmd.left.value+") not found")
							bc = [...bc, BATCHYCMDID.STACK_PUT, inputRegR, ...intTo4(height)]
						}else{
							alert("There must be only one variable on the left side of '='!")
						}
					
						return {bc:bc, stack:stack}
					}

					let left = getParsed(functionName, cmd.left, stack, astNames, {outputReg: inputRegL, inputRegL: inputRegL, inputRegR: inputRegL+1})
					//validate statement
					let ret = validateStatement(left, inputRegL, stack); bc = [...bc, ...ret.bc]; stack = ret.stack
					
					
					let right = getParsed(functionName, cmd.right, stack, astNames, {outputReg: inputRegR, inputRegL: inputRegR, inputRegR: inputRegR+1})
					//validate statement
					ret = validateStatement(right, inputRegR, stack); bc = [...bc, ...ret.bc]; stack = ret.stack
					

					switch(cmd.operator){
						case '+': bc = [...bc, BATCHYCMDID.MATH_ADD, outputReg, inputRegL,inputRegR,0,0]; break;
						case '-': bc = [...bc, BATCHYCMDID.MATH_SUB, outputReg, inputRegL,inputRegR,0,0]; break;
						case '*': bc = [...bc, BATCHYCMDID.MATH_MUL, outputReg, inputRegL,inputRegR,0,0]; break;
						case '/': bc = [...bc, BATCHYCMDID.MATH_DIV, outputReg, inputRegL,inputRegR,0,0]; break;
						case '%': bc = [...bc, BATCHYCMDID.MATH_MOD, outputReg, inputRegL,inputRegR,0,0]; break;

						case '==': bc = [...bc, BATCHYCMDID.MATH_EQ, 		outputReg, inputRegL,inputRegR,0,0]; break;
						case '!=': bc = [...bc, BATCHYCMDID.MATH_NEQ, 		outputReg, inputRegL,inputRegR,0,0]; break;
						case '&&': bc = [...bc, BATCHYCMDID.MATH_AND, 		outputReg, inputRegL,inputRegR,0,0]; break;
						case '||': bc = [...bc, BATCHYCMDID.MATH_OR, 		outputReg, inputRegL,inputRegR,0,0]; break;
						case '>': bc =  [...bc, BATCHYCMDID.MATH_GREATER, 	outputReg, inputRegL,inputRegR,0,0]; break;
						case '<': bc =  [...bc, BATCHYCMDID.MATH_SMALLER, 	outputReg, inputRegL,inputRegR,0,0]; break;
						case '>=': bc = [...bc, BATCHYCMDID.MATH_GREATEREQ, outputReg, inputRegL,inputRegR,0,0]; break;
						case '<=': bc = [...bc, BATCHYCMDID.MATH_SMALLEREQ, outputReg, inputRegL,inputRegR,0,0]; break;

						default:
							alert("Operator not implemented: "+cmd.operator)
					}
					return {bc:bc, stack:stack}
				}break;

				case "VariableDeclaration":{
					let type = cmd.defType.name
					if(cmd.defType.name != "int")
						alert("only ints are supported!")
					
					let name = cmd.name

					let value = 0
					if(cmd.value){
						value = getParsed(functionName, cmd.value, stack, astNames)
					}
					if(typeof value == "number"){	//Numbervalue
						bc = [BATCHYCMDID.REG_SET_SINGLE, 0, ...intTo4(value),		//set register 1
							BATCHYCMDID.STACK_PUSH, 0, 0, 0, 0, 0]					//push to stack
					}else if(typeof value == "string"){	//String
						//check if value is a variable
						let height = getVariableIDFromStack(stack, value)
						bc = [BATCHYCMDID.STACK_GET, 0, ...intTo4(height),			//get from stack and write to register 1
							BATCHYCMDID.STACK_PUSH, 0, 0, 0, 0, 0]					//push to stack
					}else if(typeof value == "object"){
						bc = value.bc
						stack = value.stack
						bc = [...bc, BATCHYCMDID.STACK_PUSH, 0, 0, 0, 0, 0]					//push to stack
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
			let ret = deleteAllToLastFunctioncallFromStack(stackFunc)
			stackFunc = ret.stack
			func_bc = func_bc.concat(ret.bc)	//delete all local vars and co
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
			if(BYTECODE[i+2].includes("+")){
				let step = parseInt(BYTECODE[i+2].replace("+", ""))
				let startNumberChar = intTo4(i+step)
				BYTECODE[i+2] = startNumberChar[0]
				BYTECODE[i+3] = startNumberChar[1]
				BYTECODE[i+4] = startNumberChar[2]
				BYTECODE[i+5] = startNumberChar[3]
			}else{
				let startNumber = BYTECODE_FunctionsStart[BYTECODE[i+2]];
				let startNumberChar = intTo4(startNumber)
				BYTECODE[i+2] = startNumberChar[0]
				BYTECODE[i+3] = startNumberChar[1]
				BYTECODE[i+4] = startNumberChar[2]
				BYTECODE[i+5] = startNumberChar[3]
			}
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
		if(i % 6 == 0)
			document.getElementById("Cbytecode").innerHTML += '<br><span style="color:blue;">/*'+pad(i,4)+'*/</span> '
		document.getElementById("Cbytecode").innerHTML += pad(BYTECODE[i],3)+", "
		
	}

	return BYTECODE
}

downloadFile = function(){
	let bc = bytecodeGenerate()

	function downloadString(text, fileType, fileName) {
		var blob = new Blob([text], { type: fileType });

		var a = document.createElement('a');
		a.download = fileName;
		a.href = URL.createObjectURL(blob);
		a.dataset.downloadurl = [fileType, a.download, a.href].join(':');
		a.style.display = "none";
		document.body.appendChild(a);
		a.click();
		document.body.removeChild(a);
		setTimeout(function() { URL.revokeObjectURL(a.href); }, 1500);
	}

	downloadString(new Uint8Array(bc), "text/txt", "batchy.txt")

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
