function ArticleView(data, $node) {
  var that = this;
  
  this.id = data.id;
  this.title = data.title;
  this.author = data.author;
  this.slug = data.slug;
  this.date = data.date;
  this.text = data.text;
  
  if (!$node) {
    this.$node = $("<div/>").attr("data-article-id", this.id);
    this._render();
  } else {
    this.$node = $node;
  }
  
  this.$node.delegate("h2", "click", function() {
    console.log("clicked the h2 for article %d - %s", that.id, that.title);
    that.$find("p").toggle();
  });
}

ArticleView.prototype = {
  date_formatted: function() {
    return new Date(this.date).toString().match(/^[A-Z]{3} ([A-Z]{3} \d{1,2} \d{4})/i)[1];
  },
  
  _render: function() {
    this.$node.html(Mustache.to_html(templates.article, this));
  },
  
  $find: function() {
    return this.$node.find.apply(this.$node, arguments);
  },
  
  appendTo: function(s) {
    $(s).append(this.$node);
  }
};

ArticleView.Bootstrap = function($node) {
 var json = $node.attr("data-json");
 if (json) {
   var data = JSON.parse(json);
   return new ArticleView(data, $node);
 }
 
 // could not bootstrap this node.
 return false;
}

var templates = {};
$("head script[type=text/html]").each(function() {
  templates[this.id.match(/^(.+)_tmpl$/i)[1]] = this.innerHTML;
});

$(function() {
  console.log("firing off initializer....")
  setTimeout(function() {
    console.log("initialized");
    var $articles = $("#articles");
    if (!$articles.length) {
      console.log("no server articles found, fetching new articles");
      $articles = $("<div/>").attr("id", "articles").appendTo($("#doc"));
      fetchArticles(function (articles) {
        console.log("%d articles fetched, rendering", articles.length);
        for (var i = 0; i < articles.length; i++) {
          new ArticleView(articles[i]).appendTo($articles);
        }
      });
    } else {
      console.log("%d server articles found, boot strapping", $articles.children().length);
      $articles.children().each(function() {
        ArticleView.Bootstrap($(this));
      });
    }
  }, 2000);
  
  $("#doc").delegate("a[href^=/]", "click", function(event) {
    event.preventDefault();
    window.location.hash = $(this).attr("href").slice(1);
    // TODO: might try to load the actual article with a lightweight page router
  });
});

function fetchArticles(callback) {
  setTimeout(function() {
    callback([{
      id: 5,
      title: "Quite the Article",
      author: "Ben Cherry",
      text: "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.",
      slug: "quite-the-article",
      date: 1277917085389
    }, {
      id: 4,
      title: "Fascinating Shit",
      author: "Dustin Diaz",
      text: "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.",
      slug: "fascinating-shit",
      date: 1277458434012
    }, {
      id: 3,
      title: "Hilarious Parody",
      author: "Patrick Ewing",
      text: "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.",
      slug: "hilarious-parody",
      date: 1277376890957
    }, {
      id: 2,
      title: "The Cutest LOL Cats on the Internet",
      author: "Britt Selvitelle",
      text: "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.",
      slug: "the-cutest-lol-cats-on-the-internet",
      date: 1277273575592
    }, {
      id: 1,
      title: "Incredible Insight",
      author: "Marcus Phillips",
      text: "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.",
      slug: "incredible-insight",
      date: 1277262106268
    }]);
  }, 2000);
}