var ast

//parse C to ast
function astGenerate(){
	ast = cparse(document.getElementById("Ccode").innerText);
	document.getElementById("Cast").innerText = JSON.stringify(ast, undefined, 2).replace(/ /g, "\u00a0");
}

//parse ast to bytecode
function bytecodeGenerate(){
	if(!ast) astGenerate()
	let bodyParse = function(body){
		body.forEach((cmd)=>{
			console.log(cmd)
			let type = cmd.type			//get command type
			switch(type){
				case "VariableDeclaration":
					
				break;
			}
		})
	}
	
	//loop through all functions
	ast.forEach((func)=>{
		let name = func.name			//get function name
		let type = func.defType.name	//get function type
		console.log(">function", type, name)
		bodyParse(func.body)
	})
	
}