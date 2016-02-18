using System;
using System.Data;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace EurostralWebApplication.Models.DataRetrieving
{
    public class Data
    {
        public DataSet Ds { get; set; }
        public List<string> Symbol { get; set; }
        public List<string> Columns { get; set; }
        public DateTime Start { get; set; }
        public DateTime End { get; set; }
        public DataType Type { get; set; }
    }
}