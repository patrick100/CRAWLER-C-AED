

if (window.XMLHttpRequest) ajax = new XMLHttpRequest();
else ajax = new ActiveXObject("Microsoft.XMLHTTP");

var temporal=null;

function readTextFile()
{
	window.open("file:///C:/dataweb.html")
}	

function generar()
{
    //eliminar();
	var tabla = document.getElementById("tabla_datos");
	
	if(temporal!=null)
	{
		tabla.removeChild(temporal);
	}
	
    var dato = document.getElementById('id').value;
    //alert(dato);
    ajax.open('GET', 'procesar2.php?id='+dato, false);
	ajax.send();
    //alert(ajax.responseText);
	var array = ajax.responseText.split(";");
	
	
	desple = document.getElementById('tabla_datos');
	
		
	numfilas = array.length-1;
	//alert("NUM FILAS"+numfilas);
	temporal = document.createElement('tbody');
	desple.appendChild(temporal)
	

	for(var j =0; j<numfilas; j++)
	{
		var array2=array[j].split(",");
		var tr = document.createElement('tr');
	    
			//TR PARA INGRESAR UN NUEVO ELEMENTO	
			var td1 = document.createElement('td');
			var td2 = document.createElement('td');
			
			//td.setAttribute("align", "center");
			var texto = document.createTextNode(array2[0]);
			//var texto = document.createElement('input');
			//texto.type ='text';
			//texto.value = array2[0];
			//texto.value=array[0]; //AQUIIII
			//var record = document.createTextNode(array[1]);
			var record = document.createTextNode(array2[1]);
			
			td1.appendChild(texto);
			td2.appendChild(record);	
			
		tr.appendChild(td1);
		tr.appendChild(td2);
		
		temporal.appendChild(tr);
		
		
	}
	
	desple.style.display="block";
	
	
	
}