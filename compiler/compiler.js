var ast

function astGenerate(){
	ast = cparse(document.getElementById("Ccode").innerText);
	document.getElementById("Cast").innerText = JSON.stringify(ast, undefined, 2).replace(/ /g, "\u00a0");
}

function bytecodeGenerate(){
	//parse ast to bytecode
	
}