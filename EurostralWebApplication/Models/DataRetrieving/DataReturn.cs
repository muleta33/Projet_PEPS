using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace EurostralWebApplication.Models.DataRetrieving
{
    public class DataReturn
    {
        public Data data { get; set; }
        public List<string> listeErreur { get; set; }
        public List<string> warning { get; set; }
    }
}