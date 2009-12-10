
var url = "talk.google.com:5222";
var conn = Strophe.Connection(url);
conn.connect("bcherry@gmail.com", "", function(){consul.log("%o", arguments);})