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
			switch(cmd.type){
				case "ExpressionStatement":
					return getParsed(functionName, cmd.expression, stack, astNames)
				break;

				case "CallExpression":{
					let name = cmd.base.value
					let parameter = cmd.arguments
					if(parameter.length != 1 || parameter[0] != null)
						parameter = parameter.map((c,i)=>{console.log(c,i);return getParsed(functionName, c, stack, astNames)})
					else
						parameter = []

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

				case "BinaryExpression":{
					let outputReg = args.outputReg || 0;
					let inputRegL = args.inputRegL || 1;
					let inputRegR = args.inputRegR || 2;
					let left = getParsed(functionName, cmd.left, stack, astNames, {outputReg: inputRegL, inputRegL: inputRegL, inputRegR: inputRegL+1})
					let bc = []

					if(typeof left == "number"){
						bc = [BATCHYCMDID.REG_SET_SINGLE, inputRegL, ...intTo4(left)]
					}else if(typeof left == "string"){
						let height = getVariableIDFromStack(stack, left)
						bc = [BATCHYCMDID.STACK_GET, inputRegL, ...intTo4(height)]
					}else if(typeof left == "object"){
						bc = left.bc
						stack = left.stack;
					}
					
					let right = getParsed(functionName, cmd.right, stack, astNames, {outputReg: inputRegR, inputRegL: inputRegR, inputRegR: inputRegR+1})
					if(typeof right == "number"){
						bc = [...bc, BATCHYCMDID.REG_SET_SINGLE, inputRegR, ...intTo4(right)]
					}else if(typeof right == "string"){
						let height = getVariableIDFromStack(stack, right)
						bc = [...bc, BATCHYCMDID.STACK_GET, inputRegR, ...intTo4(height)]
					}else if(typeof right == "object"){
						bc = [...bc, ...right.bc]
						stack = right.stack;
					}
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
						alter("only ints are supported!")
					
					let name = cmd.name

					let value = getParsed(functionName, cmd.value, stack, astNames)
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


		/*
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
