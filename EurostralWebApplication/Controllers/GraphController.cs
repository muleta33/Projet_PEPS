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

        //
        // GET: /Graph/
        public ActionResult Index()
        {
            ViewModelGraph model = new ViewModelGraph();
            return View(model);
        }

        [HttpPost]
        public ActionResult onChangeIndex(ViewModelGraph model) 
        {
            DateTime firstDate = new DateTime(2015, 1, 1, 0, 0, 0);
            DateTime lastDate = DateTime.Now;
            object[] values = model.getCurrentIndex().getPastPricesPeriod(firstDate, lastDate).ToArray();

            DotNet.Highstock.Highstock chart = new DotNet.Highstock.Highstock("chart")
                .InitChart(new Chart
                {
                    Type = ChartTypes.Line,
                    ClassName = "chart",
                    Events = new ChartEvents
                    {
                        Load = "ChartEventsLoad"
                    }
                })
                .AddJavascripFunction("ChartEventsLoad",
                                      @"// set up the updating of the chart each second
                                       this.addSeries({type : 'flags',
                                        data : [{
                                            x : Date.UTC(2015, 3, 30),
                                            title : '0',
                                            text : 'Niveau initial'
                                        }, {
                                            x : Date.UTC(2015, 9, 30),
                                            title : '1',
                                            text : 'Première date de constatation'
                                        }],
                                        onSeries : 'dataseries',
                                        shape : 'circlepin',
                                        width : 16
                                    });")
                .SetRangeSelector(new RangeSelector
                {
                    AllButtonsEnabled = true,
                    Selected = 1
                })
                .SetTitle(new Title
                {
                    Text = model.getCurrentIndex().Name
                })
                .SetYAxis(new YAxis
                {
                    Title = new YAxisTitle { Text = "Cours de l indice" }
                })
                .SetTooltip(new Tooltip
                {
                    ValueDecimals = 4
                })
                .SetSeries(new[]
                {
                    new Series { Name = model.getCurrentIndex().Name, Id = "dataseries", Data = new Data(values) }
                })
                ;

            return PartialView("PartialViewGraph", chart);
        }
	}
}