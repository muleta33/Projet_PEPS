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

        [Display(Name = "Fréquence (en secondes) : ")]
        public int Frequency { get; set; }
        public List<int> FrequencyOptions { get; set; }

        [Display(Name = "Nombre d'itérations de Monte-Carlo :")]
        public int NumberOfMonteCarloIterations { get; set; }

        public ViewModelPricingAndHedging()
        {
            AutomaticOrManualHedging = "Automatique";
            HedgingModes = new List<String> { "Automatique", "Manuel" };
            Frequency = 20;
            FrequencyOptions = new List<int> { 20, 40 };
            NumberOfMonteCarloIterations = 20000;
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