$(function() {
    $("#reloadform input[type=submit]").click(function() {
        $("#stories").text("reloading...");
        $.getJSON("/stories/reload", { num: $("#reloadform #numentries").val() },
            function(data) {
                if (data.success) {
                    $("#stories").text("reloaded!");
                } else {
                    $("#stories").text("error reloading");
                }
            }
        );
        return false;
    });
});