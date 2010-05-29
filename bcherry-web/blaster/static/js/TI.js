// This file provides a mock JavaScript API for a TI graphing calculator
// It doesn't work in IE, because you need canvas and reasonable key detection
// Right now, it only supports the TI-89 family, but adding support for others would be trivial

/*jslint white: true, onevar: true, browser: true, devel: true, undef: true, nomen: true, eqeqeq: true, plusplus: true, bitwise: false, regexp: true, strict: false, newcap: true, immed: true */
/*globals window: false */
var TI = (function (window) {
  var TI = {},
    calcs = {
      ti89: {
        display: {
          width: 160,
          height: 100,
          bgColor: "#D7E5D2",
          fgColor: "#313F42"
        }
      }
    },

    // Functions in this module
    assert,
    warn,
    issueWarnings,
    makeBasicDisplay,
    makeGrayDisplay,
    makeKeys;

  TI.makeCalculator = function (spec) {
    var that,
      model = spec.model,
      width,
      height,
      gfxMode = spec.gfxMode || "monochrome",
      calc,
      display;

    assert(model in calcs, "Unsupported Calculator");
    calc = calcs[model];

    assert(gfxMode === "monochrome" || gfxMode === "grayscale", "Unsupported graphics mode.");

    that = {};

    width = spec.width || calc.display.width;
    height = spec.height || calc.display.height;

    if (gfxMode === "monochrome") {
      display = that.display = makeBasicDisplay({params: calc.display, width: width, height: height});
    } else if (gfxMode === "grayscale") {
      display = that.display = makeGrayDisplay({params: calc.display, width: width, height: height});
    }

    that.getDomElement = display.getDomElement;

    that.keys = makeKeys({});

    return that;
  };

  makeBasicDisplay = function (spec) {
    var that,
      canvas,
      params = spec.params,
      width = spec.width,
      height = spec.height,
      pixelWidth,
      pixelHeight,
      bgColor = params.bgColor,
      fgColor = params.fgColor,
      noWarn = spec.noWarn,
      pixels,
      row,
      col,
      p,

      getX,
      getY;

    that = {};

    if (!noWarn) {
      issueWarnings("display", params, width, height);
    }

    canvas = document.createElement("canvas");
    canvas.width = width;
    canvas.height = height;
    pixelWidth = width / params.width;
    pixelHeight = height / params.height;

    canvas.setAttribute("style", "background-color:" + bgColor + ";border:solid 1px black;");

    that.getDomElement = function () {
      return canvas;
    };

    that.pixelOn = function (x, y, ctx) {
      ctx = ctx || canvas.getContext("2d");
      ctx.fillStyle = fgColor;
      ctx.fillRect(getX(x), getY(y), pixelWidth, pixelHeight);
    };

    that.pixelOff = function (x, y, ctx) {
      ctx = ctx || canvas.getContext("2d");
      ctx.clearRect(getX(x), getY(y), pixelWidth, pixelHeight);
    };

    that.clear = function () {
      var ctx = canvas.getContext("2d");
      ctx.clearRect(0, 0, width, height);
    };

    that.drawSprite = function (sprite, width, x, y) {
      var ctx = canvas.getContext("2d"),

        clipX = 0,
        clipY = 0,
        clippedX = x,
        clippedY = y,
        clippedWidth = width,
        clippedHeight = sprite.length,
        imageData;

      if (x < 0) {
        clipX = 0 - x;
        clippedX = 0;
        clippedWidth = width - clipX;
      } else if (x + width > that.width) {
        clippedWidth = that.width - x;
      }
      if (y < 0) {
        clipY = 0 - y;
        clippedY = 0;
        clippedHeight = sprite.length - clipY;
      } else if (y + sprite.length > that.height) {
        clippedHeight = that.height - y;
      }

      if (clippedWidth <= 0 || clippedHeight <= 0) {
        return;
      }

      if (ctx.createImageData) {
        imageData = ctx.createImageData(clippedWidth, clippedHeight);
      } else if (ctx.getImageData) {
        imageData = ctx.getImageData(clippedX, clippedY, clippedWidth, clippedHeight);
      } else {
        imageData = {width: clippedWidth, height: clippedHeight, data: Array.dim(clippedWidth * clippedHeight * 4, 0)};
      }
      pixels = imageData.data;
      for (row = 0; row < clippedHeight; row += 1) {
        for (col = 0; col < clippedWidth; col += 1) {
          if ((sprite[row + clipY] >> (width - (col + clipX) - 1)) & 1) {
            p = row * clippedWidth * 4 + col * 4;
            pixels[p] = 49;
            pixels[p + 1] = 63;
            pixels[p + 2] = 66;
            pixels[p + 3] = 255;
          }
        }
      }

      ctx.putImageData(imageData, clippedX, clippedY);
    };

    getX = function (x) {
      return x * pixelWidth;
    };

    getY = function (y) {
      return y * pixelHeight;
    };

    that.width = params.width;
    that.height = params.height;

    return that;
  };

  makeGrayDisplay = function (spec) {
    var that,
      div,
      canvas,
      params = spec.params,
      width = spec.width,
      height = spec.height,
      pixelWidth,
      pixelHeight,
      bgColor = params.bgColor,
      fgColor = params.fgColor,
      noWarn = spec.noWarn,
      pixels,
      row,
      col,
      p,

      getX,
      getY;

    that = {};

    if (!noWarn) {
      issueWarnings("display", params, width, height);
    }

    div = document.createElement("div");
    div.setAttribute("style", "width:" + width + "px;height:" + height + "px;background-color:" + bgColor + ";border:solid 1px black;position:relative;");

    canvas = document.createElement("canvas");
    canvas.width = params.width;
    canvas.height = params.height;

    canvas.setAttribute("style", "width:100%;height:100%;");

    div.appendChild(canvas);

    that.getDomElement = function () {
      return div;
    };

    that.pixelOn = function (x, y, ctx) {
      ctx = ctx || canvas.getContext("2d");
      ctx.fillStyle = params.fgColor;
      ctx.fillRect(getX(x), getY(y), pixelWidth, pixelHeight);
    };

    that.pixelOff = function (x, y, ctx) {
      ctx = ctx || canvas.getContext("2d");
      ctx.clearRect(getX(x), getY(y), pixelWidth, pixelHeight);
    };

    that.clear = function () {
      var ctx = canvas.getContext("2d");
      ctx.clearRect(0, 0, width, height);
    };

    that.drawSprite = function (layer1, layer2, width, x, y, mode) {
      var ctx = canvas.getContext("2d"),

        clipX = 0,
        clipY = 0,
        clippedX = x,
        clippedY = y,
        clippedWidth = width,
        clippedHeight = layer1.length,

        imageData,
        pixels,
        row,
        col,
        l1,
        l2,
        alpha,
        p;

      if (x < 0) {
        clipX = 0 - x;
        clippedX = 0;
        clippedWidth = width - clipX;
      } else if (x + width > that.width) {
        clippedWidth = that.width - x;
      }
      if (y < 0) {
        clipY = 0 - y;
        clippedY = 0;
        clippedHeight = layer1.length - clipY;
      } else if (y + layer1.length > that.height) {
        clippedHeight = that.height - y;
      }

      if (clippedWidth <= 0 || clippedHeight <= 0) {
        return;
      }

      if (ctx.getImageData) {
        imageData = ctx.getImageData(clippedX, clippedY, clippedWidth, clippedHeight);
      } else {
        imageData = {width: clippedWidth, height: clippedHeight, data: Array.dim(clippedWidth * clippedHeight * 4)};
      }
      pixels = imageData.data;
      for (row = 0; row < clippedHeight; row += 1) {
        for (col = 0; col < clippedWidth; col += 1) {
          l1 = (layer1[row + clipY] >> (width - (col + clipX) - 1)) & 1;
          l2 = (layer2[row + clipY] >> (width - (col + clipX) - 1)) & 1;

          alpha = l1 * 85 + l2 * 85 * 2;

          if (alpha) {
            p = row * clippedWidth * 4 + col * 4;
            pixels[p] = 49;
            pixels[p + 1] = 63;
            pixels[p + 2] = 66;
            pixels[p + 3] = alpha;
          }
        }
      }

      ctx.putImageData(imageData, clippedX, clippedY);
    };

    that.width = params.width;
    that.height = params.height;

    return that;
  };

  makeKeys = function (spec) {
    var that,
      keyMap = {
        32: "2nd",    // Space
        17: "diamond",  // Control
        16: "shift",  // Shift
        13: "enter",    // Enter/Return
        38: "up",     // Up
        40: "down",   // Down
        37: "left",   // Left
        39: "right",    // Right
        27: "esc",    // Escape
        112: "f1",      // F1
        113: "f2",      // F2
        114: "f3",      // F3
        115: "f4",      // F4
        116: "f5"     // F5
      },
      dummy = function () {
        return;
      },
      events = {
        "2nd": dummy,
        diamond: dummy,
        shift: dummy,
        enter: dummy,
        up: dummy,
        down: dummy,
        left: dummy,
        right: dummy,
        esc: dummy,
        f1: dummy,
        f2: dummy,
        f3: dummy,
        f4: dummy,
        f5: dummy
      },
      translateKeyCode,
      pressedKeys = {},
      watchKeys,
      olddown,
      oldup;

    that = {};

    that.listen = function (name, func, repeat) {
      var oldfunc;

      assert(name in events, "Unsupported key.");

      oldfunc = events[name];
      events[name] = function () {
        if (!repeat) {

        }
        if (func() === false) {
          return false;
        }
        return oldfunc();
      };
    };

    that.press = function (name) {
      assert(name in events, "Unsupported key.");

      events[name]();
    };

    that.isPressed = function (name) {
      assert(name in events, "Unsupported key.");

      return name in pressedKeys;
    };

    translateKeyCode = function (code) {
      if (!(code in keyMap)) {
        return false;
      }

      return keyMap[code];
    };

    watchKeys = function () {
      var key;

      for (key in pressedKeys) {
        if (pressedKeys.hasOwnProperty(key)) {
          events[key]();
        }
      }
    };

    olddown = document.onkeydown;
    document.onkeydown = function (e) {
      var keyCode = (window.event) ? event.keyCode : e.keyCode,
        key = translateKeyCode(keyCode);

      if (key) {
        pressedKeys[key] = true;
      }

      if (olddown) {
        return olddown();
      }
    };

    oldup = document.onkeyup;
    document.onkeyup = function (e) {
      var keyCode = (window.event) ? event.keyCode : e.keyCode,
        key = translateKeyCode(keyCode);
      if (key && pressedKeys[key] !== undefined) {
        delete pressedKeys[key];
      }

      if (oldup) {
        return oldup();
      }
    };

    setInterval(watchKeys, 100);

    return that;
  };

  // extra internal bits
  assert = function (test, msg) {
    if (!test) {
      throw "TIjs Error: " + msg;
    }
  };

  warn = function (msg) {
    if (window.console) {
      console.warn("TIjs Warning: " + msg);
    }
  };

  issueWarnings = function (component, params, width, height) {
    if (component.toLowerCase() === "display") {
      if (width < params.width) {
        warn("Display width is too small, some pixels will be lost.");
      } else if (width % params.width !== 0) {
        warn("Display width is not even multiple of native width and will look bad.");
      }
      if (height < params.height) {
        warn("Display height is too small, some pixels will be lost");
      } else if (height % params.height !== 0) {
        warn("Display height is not even multiple of native height and will look bad.");
      }
      if (width / height !== params.width / params.height) {
        warn("Display aspect ratio is off, dome distortion will occur.");
      }
    }
  };

  return TI;
}(window));