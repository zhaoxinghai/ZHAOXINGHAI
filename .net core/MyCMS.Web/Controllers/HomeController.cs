using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using MyCMS.Web.Models;

namespace MyCMS.Web.Controllers
{
    public class HomeController : Controller
    {
        public int test = 0;
        public IActionResult Index()
        {
            test++;

            int a = 3 + 4;
            return View();
        }

        public IActionResult About()
        {
            test++;
            int a = 3 + 4;
            ViewData["Message"] = "Your application description page.";

            return View();
        }

        public IActionResult Contact()
        {
            ViewData["Message"] = "Your contact page.";

            return View();
        }

        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }
}
