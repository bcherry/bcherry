using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Net;
using System.Xml;
using System.Data;

namespace FMySemanticLife.Engine
{
    public static class FMyService
    {
        public static bool ReloadStories(int num)
        {
            Engine.ClearAll();

            for (int i = 0; i < num; i++)
            {
                var reader = new XmlTextReader("http://api.betacie.com/view/last/" + i + "?key=readonly&language=en");
                var ds = new DataSet();
                ds.ReadXml(reader);
                foreach(DataRow row in ds.Tables[2].Rows)
                {
                    string text = row.Field<string>("text");
                    int id = int.Parse(row.Field<string>("id"));
                    Engine.StoreStory(id, text);
                }

            }

            return true;
        }
    }
}
