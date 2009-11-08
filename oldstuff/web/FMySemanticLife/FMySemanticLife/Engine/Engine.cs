using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Text.RegularExpressions;

namespace FMySemanticLife.Engine
{
    public static class Engine
    {
        /// <summary>
        /// Store a new document or update an existing one
        /// </summary>
        /// <param name="documentID"></param>
        /// <param name="body"></param>
        public static void StoreStory(int storyID, string text)
        {
            using (var _db = new EngineDBDataContext())
            {
                IList<Term> terms = TermSet(text);

                // grab idfs
                var idfs = from term in _db.Terms
                           where term.StoryID != storyID
                           group term by term.Text
                               into g
                               where terms.Select(t => t.Text).Contains(g.Key)
                               select new { TermVal = g.Key, Count = g.Count() };

                // calculate tfidfs
                double num_docs = (from term in _db.Terms
                                   where term.StoryID != storyID
                                   group term by term.StoryID
                                       into g
                                       select g).Count() + 1;
                foreach (Term term in terms)
                {
                    term.StoryID = storyID;
                    Term term1 = term;
                    var idf = 1;
                    // need to refactor this because it's doing a massive SELECT * FROM tbl WHERE x IN ...
                    var idfEntry = idfs.SingleOrDefault(t => t.TermVal.ToLower() == term1.Text.ToLower());
                    if (idfEntry != null)
                        idf = idfEntry.Count;
                    term.tfidf = term.tf * Math.Log(num_docs / idf);
                }

                // delete old entries
                _db.Terms.DeleteAllOnSubmit(_db.Terms.Where(t => t.StoryID == storyID));

                // store
                _db.Terms.InsertAllOnSubmit(terms);

                _db.SubmitChanges();
            }

            ScoreStory(storyID);
        }

        /// <summary>
        /// Get the documents most related to documentID
        /// </summary>
        /// <param name="documentID"></param>
        /// <param name="currentPageIndex"></param>
        /// <param name="pageSize"></param>
        /// <returns></returns>
        public static IList<int> GetSimilarStories(int storyID, int pageSize, int currentPageIndex)
        {
            using (var _db = new EngineDBDataContext())
            {
                var q = from score in _db.Scores
                        where score.Story1ID == storyID || score.Story2ID == storyID
                        orderby score.Value descending
                        select score.Story1ID == storyID ? score.Story1ID : score.Story2ID;

                return q.Skip(currentPageIndex * pageSize).Take(pageSize).ToList();
            }
        }

        /// <summary>
        /// For each document, sum the weights of that document's terms that occur in 
        /// 'searchTerms' and return matches that exceed a certain threshold.
        /// </summary>
        /// <param name="searchTerms"></param>
        /// <param name="currentPageIndex"></param>
        /// <param name="pageSize"></param>
        /// <returns></returns>
        public static IList<int> SearchStories(string[] searchTerms, int pageSize, int currentPageIndex)
        {
            using (var _db = new EngineDBDataContext())
            {

                var q = from term in _db.Terms
                        where searchTerms.Contains(term.Text)
                        group term by term.StoryID
                            into dterms
                            orderby dterms.Sum(t => t.tf) descending
                            select dterms.Key;

                return q.Skip(currentPageIndex * pageSize).Take(pageSize).ToList();
            }
        }

        public static IList<int> SearchStories(string[] searchTerms, int pageSize, int currentPageIndex, int[] exclude)
        {
            using (var _db = new EngineDBDataContext())
            {

                var q = from term in _db.Terms
                        where searchTerms.Contains(term.Text)
                        group term by term.StoryID
                            into dterms
                            orderby dterms.Sum(t => t.tf) descending
                            select dterms.Key;

                return q.Where(x => !exclude.Contains(x)).Skip(currentPageIndex * pageSize).Take(pageSize).ToList();
            }
        }

        public static void RemoveStory(int storyID)
        {
            using (var _db = new EngineDBDataContext())
            {
                _db.Terms.DeleteAllOnSubmit(_db.Terms.Where(t => t.StoryID == storyID));

                _db.Scores.DeleteAllOnSubmit(_db.Scores.Where(s => s.Story1ID == storyID || s.Story2ID == storyID));

                _db.SubmitChanges();
            }
        }

        public static void ClearAll()
        {
            using (var _db = new EngineDBDataContext())
            {
                _db.Terms.DeleteAllOnSubmit(_db.Terms);
                _db.Scores.DeleteAllOnSubmit(_db.Scores);
                _db.SubmitChanges();
            }
        }

        public static void RescoreAll()
        {
            using (var _db = new EngineDBDataContext())
            {
                var ids = from term in _db.Terms
                          group term by term.StoryID
                              into g
                              select g.Key;

                foreach (int id in ids)
                {
                    ScoreStory(id);
                }
            }
        }

        /* Calculate the cosine similarity measure of ID versus all other documents in the corpus */

        private static void ScoreStory(int storyID)
        {
            // consider passing data context as an argument to the function
            using (var _db = new EngineDBDataContext())
            {

                // delete all existing scores for documentID
                _db.Scores.DeleteAllOnSubmit(from score in _db.Scores
                                             where score.Story1ID == storyID || score.Story2ID == storyID
                                             select score);
                _db.SubmitChanges();

                // grab the principle document's terms/tfidfs
                var terms = from term in _db.Terms
                            where term.StoryID == storyID
                            select new { Text = term.Text, tfidf = term.tfidf };

                // magnitude of the document's vector
                var mag1 = Math.Sqrt(terms.Sum(t => t.tfidf));

                // ids of documents to score against
                var ids = from term in _db.Terms
                          where term.StoryID != storyID
                          group term by term.StoryID
                              into g
                              select g.Key;

                // score against each document
                foreach (int id in ids)
                {
                    var id1 = id;
                    // grab the other document's terms/tfidfs
                    var terms2 = from term in _db.Terms
                                 where term.StoryID == id1
                                 select new { Text = term.Text, tfidf = term.tfidf };

                    var mag2 = Math.Sqrt(terms2.Sum(t => t.tfidf));

                    // calculate the dot product
                    var dot_product = 0.0;
                    foreach (var term in terms)
                    {
                        var term1 = term;
                        var term2 = terms2.SingleOrDefault(t => t.Text.ToLower() == term1.Text.ToLower());
                        if (term2 != null)
                            dot_product += term1.tfidf * term2.tfidf;
                    }

                    var euclidean_dist = mag1 * mag2;

                    // linq may complain about overwriting existing keys... we'll see. if so, delete scores before InsertOnSubmit()
                    var score = new Score
                    {
                        Story1ID = storyID,
                        Story2ID = id,
                        Value = dot_product / euclidean_dist
                    };
                    if (double.IsNaN(score.Value)) score.Value = 0;
                    _db.Scores.InsertOnSubmit(score);
                }

                _db.SubmitChanges();

                // 
            }

        }


        /* Take a string 'str', return a hashtable of (term,frequency) pairs,
         * where term is unique and frequency is the word count of term (# of occurences) in 'str'
         * divided by the total word count of 'str'
         */

        private static IList<Term> TermSet(string str)
        {
            // clean the body string
            string[] term_list = str.ToCleanStringArray();

            // create a set of unique terms with associated counts
            IList<Term> term_set = new List<Term>();
            foreach (string term in term_list)
            {
                if (term.IsTooCommon())
                    continue;
                string term1 = term;
                if (term_set.Count(t => t.Text.Equals(term1, StringComparison.CurrentCultureIgnoreCase)) > 0)
                {
                    term_set.Single(t => t.Text.Equals(term1, StringComparison.CurrentCultureIgnoreCase)).tf++;
                }
                else
                {
                    term_set.Add(new Term
                    {
                        Text = term,
                        tf = 1.0,
                        tfidf = 0.0
                    });
                }
            }

            term_set = term_set.OrderByDescending(x => x.tf).Take(50).ToList();
            term_set.ForEach(term => term.tf /= term_list.Length);

            return term_set;
        }

        private static string[] ToCleanStringArray(this string str)
        {
            return Regex.Replace(str.ToLower(), @"\W+", " ").Trim().Split(new[] { ' ', '\t', '\r', '\n' });
        }

        /// <summary>
        /// Extension method to check strings for being too common, such as articles or simple words.  Needs work.
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        private static bool IsTooCommon(this string str)
        {
            switch (str)
            {
                case "a":
                case "A":
                case "I":
                case "is":
                case "the":
                case "an":
                case "An":
                case "The":
                case "of":
                case "Of":
                case "to":
                case "To":
                case "Today":
                case "FML":
                    return true;
                default:
                    return false;
            }
        }

        public static IEnumerable<T> ForEach<T>(this IEnumerable<T> source, Action<T> action)
        {
            foreach (var item in source)
                action(item);
            return source;
        }
    }
}