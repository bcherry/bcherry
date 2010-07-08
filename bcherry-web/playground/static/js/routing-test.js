R.dontChangeUrl(); // disables URL changes

test("R creates a route", function() {
  expect(3);
  
  R("home", "/", function() {
    ok(true, "routed correctly")
  });
  
  equals(R.urlFor.home(), "/", "urlFor works");
  
  R.go("home");
  R.toUrl("/home");
});
// 
// R("home", "/", function() {});
// R("mentions", "/mentions", function() {});
// R("replies", "/replies", function() {});
// R("profile", "/:screenName", function(screenName) {});
// R("tweet", "/:screenName/statuses/:id", function(screenName, id) {});
// 
// R.urlFor.profile({
//   screenName: "bcherry"
// }); // "/bcherry"
// 
// R.urlFor.tweet({
//   screenName: "bcherry",
//   id: 123456
// }); // "/bcherry/statuses/123456"
// 
// R.go.profile({
//   screenName: "bcherry"
// });
// 
// R.toUrl("/bcherry");

