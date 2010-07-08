var R = (function() {
  var routes = [],
      noUrlChanges = true;
      
  
  function R(name, url, fn) {
    var args = [];
    if (R.urlFor.name) {
      throw "Routing Error: previous defined or illegal route name: " + name;
    }
    
    var splits = url.split("/");
    for (var i = 0; i < splits.length; i++) {
      var piece = splits[i];
      if (piece.charAt(0) === ":") {
        args.push(piece.slice(1));
      }
    }
    
    var route = {
      name: name,
      url: url,
      fn: fn,
      _splits: splits,
      _args: args
    };
    
    routes.push(route);
    R.urlFor[name] = generateUrlFor(route);
    R.go[name] = generateGo(route);
  }
  
  function _urlFor(params) {
    return this.url;
  }
  
  function _go(params) {
    var i,
        args = [];
    for (i = 0; i < this._args.length; i++) {
      args.push(params[this._args[i]]);
    }
    
    this.fn.apply(null, args);
    
    // TODO: url updating
  }
  
  function generateUrlFor(route) {
    return function(params) {
      return _urlFor.call(route, params);
    };
  }
  
  function generateGo(route) {
    return function(params) {
      return _go.call(route, params);
    };
  }
  
  R.urlFor = function(name, params) {
    return R.urlFor[name](params);
  };
  
  R.go = function(name, params) {
    return R.go[name](params);
  };
  
  R.toUrl = function(url) {
    var splitUrl = url.split("/");
    for (var i = 0; i < routes.length; i++) {
      var route = routes[i],
        splitRoute = route._splits,
        found = false;
      
      if (splitRoute.length === splitUrl.length) {
        found = true;
        var args = [];
        for (var j = 0; j < splitRoute.length; j++) {
          if (splitRoute[j].charAt(0) === ":") {
            args.push(splitUrl[j]);
          } else if (splitRoute[j] !== splitUrl[j]) {
            found = false;
          }
        }
      }
      
      if (found) {
        return route.fn.apply(null, args);
      }
    }
  };
  
  // Testing mode function that doesn't change URLs, to avoid messing up e.g. qUnit
  R.dontChangeUrl = function() {
    noUrlChanges = true;
  };
  
  return R;
}());