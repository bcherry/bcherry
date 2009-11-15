;(function(){
	var threadManager = {
		seed: 0,
		current: 0,
		threads: [],
		start: function() {
			if (this.current) {
				return;
			}
			(function(){
				for (var i = 0; i < threadManager.threads.length; i++ ) {
					if (threadManager.threads[i].run() === false ) {
						threadManager.threads[i].stop(true);
						threadManager.threads.splice(i,1);
						i--;
					}
				}
				threadManager.current = setTimeout(arguments.callee, 0);
			})();
		},
		stop: function() {
			clearTimeout(this.current);
			this.current = 0;
		},
		add: function(thread) {
			thread.__threadID = this.seed++;
			this.threads.push(thread);
			this.start();
			return thread.__threadID;
		},
		remove: function(threadID) {
			for (var i = 0; i < threadManager.threads.length; i++) {
				if (threadManager.threads[i].__threadID == threadID) {
					threadManager.threads.splice(i,1);
				}
			}
		}
	};

	this.SimpleThread = function(workFn,params) {
		var that = this;
		var autoStart = (params && params.autoStart !== null) || true;
		var workArgs = (params && params.workArgs) || [];
		var _log = (params && params.log !== null) || false;
		var workFn = workFn;
		var threadID = null;
		var _callee = arguments.callee;

		var log = function(msg) {
			if (_log) {
				console.log("SimpleThread #" + threadID + ": " + msg);
			}
		}

		// Control functions
		this.start = function() {
			if (!this.isRunning()) {
				threadID = threadManager.add(that);
				log("started");
			}
		};
		this.stop = function(skipThreadManager) {
			if (!skipThreadManager) {
				threadManager.remove(threadID);
			}
			if (that.isRunning()) {
				log("stopped");
				threadID = null;
			}
		};
		this.isRunning = function() {
			return threadID !== null;
		};
		this.run = function() {
			return workFn.apply(_callee,workArgs);
		};

		// Getters/Setters
		this.getBatchSize	= function()	{ return batchSize; };
		this.setBatchSize	= function(n)	{ batchSize = n; };
		this.getWorkFn		= function() 	{ return workFn; };
		this.setWorkFn		= function(fn)	{ workFn = fn; };
		this.getWorkArgs	= function()	{ return workArgs; };
		this.setWorkArgs	= function(args){ workArgs = args; };
		this.getThreadID	= function()	{ return threadID; };

		if (autoStart) {
			this.start();
		}
	};
})();

