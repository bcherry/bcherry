;(function(){
	var console = AGD;
	
	var calcParams = {
		width: 640,
		height: 400
	};
	var calc = new TI.Calculator("ti89", calcParams);
	
	var init = function() {
		document.body.appendChild(calc.display.getDomElement());
		
		for (var x = 0; x < 100; x++) {
			calc.display.pixelOn(x, x);
			if (x % 2 === 0) {
				calc.display.pixelOff(x, x);
			}
		}
	};
	
	$(init);
})();