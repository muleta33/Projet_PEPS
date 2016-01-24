using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Web.Optimization;
using System.Web.Routing;

namespace EurostralWebApplication
{
    public class MvcApplication : System.Web.HttpApplication
    {
        void Application_Start(object sender, EventArgs e)
        {
            String _path = String.Concat(System.Environment.GetEnvironmentVariable("PATH"), ";", System.AppDomain.CurrentDomain.RelativeSearchPath);
            System.Environment.SetEnvironmentVariable("PATH", _path, EnvironmentVariableTarget.Process);
            AreaRegistration.RegisterAllAreas();
            FilterConfig.RegisterGlobalFilters(GlobalFilters.Filters);
            RouteConfig.RegisterRoutes(RouteTable.Routes);
            BundleConfig.RegisterBundles(BundleTable.Bundles);
        }
    }
}
