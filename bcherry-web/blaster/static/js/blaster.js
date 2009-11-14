;(function(){
	var console = AGD;
	var calc = new TI.Calculator("ti89");
	
	var init = function() {		
		document.body.appendChild(calc.getDomElement());
	};
	
	$(init);
})();