using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace FMySemanticLife.Models
{
    public class Story
    {
        public int ID { get; set; }
        public string Author { get; set; }
        public string Category { get; set; }
        public string Text { get; set; }
    }
}