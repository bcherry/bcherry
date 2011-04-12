(function(global) {
  var testContext;

  function mock(object, property, mockedValue) {
    testContext.mocks.push({
      object: object,
      property: property,
      mockedValue: mockedValue,
      originalValue: object[property],
      hadOwnProperty: object.hasOwnProperty(property)
    });
    object[property] = mockedValue;
  }

  function unmockTheMocks() {
    for (var i = 0; i < testContext.mocks.length; i++) {
      var mock = testContext.mocks[i];
      if (mock.hadOwnProperty) {
        mock.object[mock.property] = mock.originalValue;
      } else {
        try {
          delete mock.object[mock.property];
        } catch (e) {
          // not perfect, I guess
          mock.object[mock.property] = null;
        }
      }
    }
  }

  global.setupFramework = function() {
    testContext = {
      mocks: []
    };
    mock(global, "mock", mock);
  };

  global.teardownFramework = function() {
    unmockTheMocks();
    testContext = null;
  };

  global.usingFramework = function(fn) {
    setupFramework();
    fn();
    teardownFramework();
  };

}(window));