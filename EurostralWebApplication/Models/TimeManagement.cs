using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace EurostralWebApplication.Models
{
    public class TimeManagement
    {
        public static double convertCurrentTimeInDouble(DateTime beginDate)
        {
            return (DateTime.Now - beginDate).Days / 365.0;
        }

        public static bool isAtObservationDate(double currentTime)
        {
            return ((currentTime * 2) % 1.0) < 1 / 365;
        }
    }
}