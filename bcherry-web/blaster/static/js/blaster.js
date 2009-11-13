;(function(){
	var console = AGD;
	var canvas;
	var draw = function() {
		var context = canvas.getContext("2d");
		context.globalCompositeOperation = 'destination-over';
		context.clearRect(0,0,300,300);

		context.fillRect(10,10,10,10);
		
		setTimeout(arguments.callee, 0);
	};
	
	var init = function() {
		canvas = document.createElement("canvas");
		canvas.width = 800;
		canvas.height = 500;
		canvas.id = "blaster";
		document.body.appendChild(canvas)
		draw();
	};
	
	$(init);
})();