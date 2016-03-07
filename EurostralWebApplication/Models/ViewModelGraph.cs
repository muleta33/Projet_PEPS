using System;
using System.Collections.Generic;
using System.Web.Mvc;
using System.Linq;
using System.Web;
using DotNet.Highstock.Enums;
using DotNet.Highstock.Helpers;
using DotNet.Highstock.Options;
using System.ComponentModel.DataAnnotations;

namespace EurostralWebApplication.Models
{
    public class ViewModelGraph
    {

        private readonly List<Index> underlyings;

        [Display(Name = "Choix de l'indice : ")]
        public String selectedUnderlyingId { get; set; }

        public ViewModelGraph()
        {
            underlyings = new List<Index> { new Index("Euro Stoxx 50", "STOXX50E"), new Index("SP ASX 200", "AXJO"), new Index("SP 500", "GSPC") };
            selectedUnderlyingId = "STOXX50E";
        }

        public IEnumerable<SelectListItem> underlyingsItems
        {
            get { return new SelectList(underlyings, "YahooFinanceName", "Name"); }
        }

        public Index getCurrentIndex()
        {
            return underlyings.Find(x => x.YahooFinanceName == selectedUnderlyingId);
        }

    }

}