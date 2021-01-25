var ast

//parse C to ast
function astGenerate(){
	ast = cparse(document.getElementById("Ccode").innerText);
	document.getElementById("Cast").innerText = JSON.stringify(ast, undefined, 2).replace(/ /g, "\u00a0");
}

//parse ast to bytecode
function bytecodeGenerate(){
	if(!ast) astGenerate()
		
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
	
	let bodyParse = function(body){
		let func_bc = []
		body.forEach((cmd)=>{
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
					bc = lookupFuncs[expression](arguments)
					console.log(expression, arguments, bc)
				break;
			}
			func_bc = func_bc.concat(bc)
		})
		return func_bc
	}
	
	//loop through all functions
	let full_bc = {}
	ast.forEach((func)=>{
		let name = func.name			//get function name
		let type = func.defType.name	//get function type
		console.log(">function", type, name)
		full_bc[name] = bodyParse(func.body)
	})
	document.getElementById("Cbytecode").innerText = JSON.stringify(full_bc)
}

let intTo4 = function(value){
	return [value&0xFF, (value>>8)&0xFF, (value>>16)&0xFF, (value>>24)&0xFF]
}
