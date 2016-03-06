using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using DotNet.Highstock.Enums;
using DotNet.Highstock.Helpers;
using DotNet.Highstock.Options;
using EurostralWebApplication.Models;

namespace EurostralWebApplication.Controllers
{
    public class GraphController : Controller
    {

        public static Index[] Indexes = new Index[3] { new Index("Euro Stoxx 50", "STOXX50E"), new Index("SP ASX 200", "AXJO"), new Index("SP 500", "GSPC") };

        //
        // GET: /Graph/
        public ActionResult Index()
        {

            List<object[]> values_List = new List<object[]>();
            DateTime firstDate = new DateTime(2015, 5, 2, 0, 0, 0);
            DateTime lastDate = new DateTime(2016, 3, 2, 0, 0, 0);
            object[] values = Indexes[0].getPastPricesPeriod(firstDate, lastDate).ToArray();

            DotNet.Highstock.Highstock chart = new DotNet.Highstock.Highstock("chart")
                .InitChart(new Chart
                {
                    Type = ChartTypes.Line,
                    ClassName = "chart"
                })
                .SetRangeSelector(new RangeSelector
                {
                    AllButtonsEnabled = true,
                    Selected = 1
                })
                .SetTitle(new Title
                {
                    Text = "USD to EUR exchange rate"
                })
                .SetYAxis(new YAxis
                {
                    Title = new YAxisTitle { Text = "Exchange rate" }
                })
                .SetTooltip(new Tooltip
                {
                    ValueDecimals = 4
                })
                .SetSeries(new[]
                {
                    new Series { Name = "USD to EUR", Id = "dataseries", Data = new Data(values) }
                })
                ;

            return View(chart);

        }
	}
}