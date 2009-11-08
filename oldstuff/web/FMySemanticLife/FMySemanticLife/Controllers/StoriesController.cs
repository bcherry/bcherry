using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Web.Mvc.Ajax;
using FMySemanticLife.Engine;

namespace FMySemanticLife.Controllers
{
    public class StoriesController : Controller
    {
        public ActionResult Reload(int num)
        {
            bool r = FMyService.ReloadStories(num);

            return new JsonResult { Data = new { success = r } };
        }

    }
}
