$(function () {
  var keyboardShortcuts = {
    "j": next,
    "k": prev,
    "f": fav,
    "r->t": retweet,
    "r->p": reply,
    "meta+z": undo,
    "meta+y": redo
  };

  var history = [];
  var antiHistory = [];
  function addToHistory(item) {
    antiHistory = [];
    history.push(item);
  }

  function next() {
    var $current = $("#stream .focused");
    if ($current.length === 0) {
      $("#stream .tweet:first").addClass("focused");
    } else {
      var $next = $current.next();
      if ($next.length) {
        $current.removeClass("focused");
        $next.addClass("focused");
      }
    }
  }

  function prev() {
    var $current = $("#stream .focused");
    if ($current.length === 0) {
      $("#stream .tweet:first").addClass("focused");
    } else {
      var $prev = $current.prev();
      if ($prev.length) {
        $current.removeClass("focused");
        $prev.addClass("focused");
      }
    }
  }

  function fav() {
    var $current = $("#stream .focused");

    if ($current.length) {
      doFav($current);
    }


    addToHistory({
      tweet: $current.attr("id"),
      action: "fav"
    });
  }

  function doFav($tweet) {
    var $fav = $tweet.find(".fav");
    if ($fav.length) {
      $fav.remove();
    } else {
      $tweet.append($("<span/>").addClass("fav").text("*"));
    }
  }

  function retweet() {
    var $current = $("#stream .focused");

    if ($current.length) {
      doRetweet($current);
    }

    addToHistory({
      tweet: $current.attr("id"),
      action: "rt"
    });
  }

  function doRetweet($tweet) {
    var $rt = $tweet.find(".retweeted");
    if ($rt.length) {
      $rt.remove();
    } else {
      $tweet.append($("<span/>").addClass("retweeted").text("retweeted"));
    }
  }

  function reply() {
    var $current = $("#stream .focused");

    if (!$current.length) {
      return;
    }

    function submit() {
      $("#tweet-box").val('').unbind("keydown", submit);
      $current.append($("<span/>").addClass("replied").text("replied"));
      addToHistory({
        tweet: $current.attr("id"),
        action: "rp"
      });
    }

    $("#tweet-box").val("re " + $($current).attr("id") + ": ").bind("keydown", "return", submit).focus();
  }

  function undo() {
    var item = history.pop();
    if (!item) {
      return;
    }

    var $tweet = $("#" + item.tweet);
    if (item.action === "fav") {
      doFav($tweet);
    } else if (item.action === "rt") {
      doRetweet($tweet);
    } else if (item.action === "rp") {
      $tweet.find(".replied:last").remove();
    }

    antiHistory.push(item);
  }

  function redo() {
    var item = antiHistory.pop();
    if (!item) {
      return;
    }

    var $tweet = $("#" + item.tweet);
    if (item.action === "fav") {
      doFav($tweet);
    } else if (item.action === "rt") {
      doRetweet($tweet);
    } else if (item.action === "rp") {
      $tweet.append($("<span/>").addClass("replied").text("replied"));
    }

    history.push(item);
  }

  for (var keys in keyboardShortcuts) {
    var action = keyboardShortcuts[keys];

    keys = keys.split("->");
    if (keys.length === 1) {
      keys = keys[0];
    }
    $(document).bind("keypress", keys, action);
  }

  $("#tweet-box").bind("keydown", "return", function () {
    $("#tweet-box").blur();
  });

  $("#stream").delegate(".tweet", "click", function () {
    $("#stream .focused").removeClass("focused");
    $(this).addClass("focused");
  });
});