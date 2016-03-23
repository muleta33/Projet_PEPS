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
        public Portfolio Portfolio { get; set; }

        public String AutomaticOrManualHedging { get; set; }
        public List<String> HedgingModes { get; set; }

        public bool IsInStartMode { get; set; }

        [Display(Name = "Fréquence : ")]
        public int Frequency { get; set; }

        public List<int> FrequencyOptions { get; set; }

        public bool IsHedgingInitialized { get; set; }

        public ViewModelPricingAndHedging()
        {
            IsInStartMode = true;
            AutomaticOrManualHedging = "Automatique";
            HedgingModes = new List<String> { "Automatique", "Manuel" };
            Frequency = 45;
            FrequencyOptions = new List<int> { 45, 60 };
            IsHedgingInitialized = false;
            Portfolio = new Portfolio(3);
        }

        public IEnumerable<SelectListItem> FrequencyOptionsItems
        {
            get { return new SelectList(FrequencyOptions); }
        }

        public bool isInAutomaticMode()
        {
            return (AutomaticOrManualHedging == "Automatique");
        }
    }
}