using System;
using System.Collections.Generic;
using System.Web.Mvc;
using System.Linq;
using System.Web;
using System.ComponentModel.DataAnnotations;

namespace EurostralWebApplication.Models
{
    public class ViewModelTestPL
    {

        private readonly List<TestMode> testModes;

        [Display(Name = "Choix du test : ")]
        public String selectedTestMode { get; set; }

        public ViewModelTestPL()
        {
            testModes = new List<TestMode> { new TestMode("BackTest", "Back Test"), new TestMode("ForwardTest", "Forward Test") };
            selectedTestMode = "BackTest";
        }

        public IEnumerable<SelectListItem> modeItems
        {
            get { return new SelectList(testModes, "Id", "Name"); }
        }

        public TestMode getCurrentMode()
        {
            return testModes.Find(x => x.Id == selectedTestMode);
        }

    }
}