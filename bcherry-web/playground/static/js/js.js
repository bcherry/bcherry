;(function($){
	init();
	
	var someVar = {a:1, b:2};
	var someString = "hello, world";
	
	AGD.registerVar("someVar", some);
	
	function init(){
		var AGD = window.AGD = {};
		
		var registeredVars = [];
		
		AGD.registerVar = function(v) {
			registeredVars.shift(v);
		}
	
	}
})(jQuery);