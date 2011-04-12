module("framework");

test("mock", function() {
  var foo = {bar: 1};
  usingFramework(function() {
    mock(foo, "bar", 2);
    equal(foo.bar, 2, "mocked property works");
  });

  equal(foo.bar, 1, "unmocked after ending the usingFramework block");
});