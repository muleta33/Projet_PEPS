using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace EurostralWebApplication.Models
{
    public class TestMode
    {
        public String Id { get; set; }
        public String Name { get; set; }
        public Double CurrentPL { get; set; }

        public TestMode(String id, String name)
        {
            this.Id = id;
            this.Name = name;
            this.CurrentPL = 0;
        }

        public List<object[]> getRandomPLData()
        {
            // Choix d'un fichier aléatoire
            Random rnd = new Random();
            int randVal = rnd.Next(1, 10);
            String fileName = "eurostralMutualFund" + this.Id + randVal + ".csv";
            // Parsing du fichier
            string line;

            // Read the file and display it line by line.
            String path = HttpContext.Current.Server.MapPath("/TestsPL/");
            System.IO.StreamReader file =
                new System.IO.StreamReader(path+fileName);
            List<object[]> values = new List<object[]>();
            int i = 0;
            Double spot = 0;
            string[] data = null;
            while (((line = file.ReadLine()) != null) && (i < 2096))
            {
                data = line.Split(';');
                if (i == 0)
                {
                    spot = Convert.ToDouble(data[1].ToString().Replace(".", ","));
                }
                values.Add(new object[] { Convert.ToDateTime(data[0]).ToUniversalTime(), Convert.ToDouble(data[1].ToString().Replace(".", ",")), Convert.ToDouble(data[2].ToString().Replace(".", ",")) });
                i++;
            }
            CurrentPL = 100 * (Convert.ToDouble(data[1].ToString().Replace(".", ",")) - Convert.ToDouble(data[2].ToString().Replace(".", ","))) / spot;
            file.Close();
            return values;
        }
    }
}