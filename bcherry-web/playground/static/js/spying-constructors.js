function spy(F) {
	function G() {
		var args = Array.prototype.slice.call(arguments);
		G.calls.push(args);
		F.apply(this, args);
	}
	
	G.prototype = F.prototype;
	G.calls = [];
	
	return G;
}

function verify(F) {
	return function () {
		var args = Array.prototype.slice.call(arguments),
			i,
			j,
			call,
			count = 0,
			matched;
		
		for (i = 0; i < F.calls.length; i += 1) {
			call = F.calls[i];
			matched = true;
			for (j = 0; j < args.length; j += 1) {
				if (args[j] !== call[j]) {
					matched = false;
					break;
				}
			}
			if (matched) {
				count += 1;
			}
		}
		
		return count > 0;
	};
}

function Foo(name, id) {
	this.name = name;
	this.id = id;
}

Foo.prototype = {
	log: function () {
		console.log("Foo %o:%o", this.id, this.name);
	}
};

var f = new Foo("test", 1);
f.log();

Foo = spy(Foo);

var f2 = new Foo("spied", 2);
f2.log();

console.log("verify Foo(\"spied\", 2): %o", verify(Foo)("spied", 2));
console.log("verify Foo(\"something\", 2): %o", verify(Foo)("something", 2));