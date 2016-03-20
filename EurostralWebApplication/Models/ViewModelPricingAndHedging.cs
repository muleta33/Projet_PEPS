﻿using System;
using System.Collections.Generic;
using System.Web.Mvc;
using System.Linq;
using System.Web;
using System.ComponentModel.DataAnnotations;

namespace EurostralWebApplication.Models
{
    public class ViewModelPricingAndHedging
    {
        public String AutomaticOrManualHedging { get; set; }
        public List<String> HedgingModes { get; set; }

        [Display(Name = "Fréquence : ")]
        public int Frequency { get; set; }
        public List<int> FrequencyOptions { get; set; }

        public ViewModelPricingAndHedging()
        {
            AutomaticOrManualHedging = "Automatique";
            HedgingModes = new List<String> { "Automatique", "Manuel" };
            Frequency = 30;
            FrequencyOptions = new List<int> { 15, 30 };
        }

        public IEnumerable<SelectListItem> FrequencyOptionsItems
        {
            get { return new SelectList(FrequencyOptions); }
        }

        public bool isAutomaticHedging()
        {
            if (AutomaticOrManualHedging == "Automatique")
                return true;
            return false;
        }
    }
}