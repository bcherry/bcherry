(function () {
  var iframe = document.createElement("iframe");
  iframe.src = "";
  iframe.style.display = "none";
  document.body.appendChild(iframe);
  iframe.contentWindow.document.write("\
  <html>\
  <head>\
  </head>\
  <body>\
  <script>\
    this.parent.safe_eval = function (code) {\
      return eval(code);\
    };\
    this.onbeforeunload = function () { return false; };\
  </script>\
  </body>\
  </html>\
  ");
  safe_eval("this.parent = this.top = null;")
}());