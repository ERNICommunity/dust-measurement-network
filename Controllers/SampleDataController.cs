using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;

namespace hh_fe.Controllers
{
    [Route("api/[controller]")]
    public partial class SampleDataController : Controller
    {
        private static string[] Summaries = new[]
        {
            "Freezing", "Bracing", "Chilly", "Cool", "Mild", "Warm", "Balmy", "Hot", "Sweltering", "Scorching"
        };

        [HttpGet("[action]")]
        public IEnumerable<Marker> Points()
        {
            return new[] {
                new Marker(48.142402, 17.108669, 5, new DateTimeOffset(new DateTime(2018, 8, 20, 12, 0, 0), TimeSpan.FromHours(2))),
                new Marker(47.008323, 8.486309, 2, new DateTimeOffset(new DateTime(2018, 7, 15, 1, 0, 0), TimeSpan.FromHours(1))),
                new Marker(14.598882, 120.971040, 10, new DateTimeOffset(new DateTime(2018, 5, 1, 22, 30, 0), TimeSpan.FromHours(8))),
                new Marker(47.370677, 8.540103, 6, new DateTimeOffset(new DateTime(2018, 8, 22, 15, 30, 0), TimeSpan.FromHours(1))),
                new Marker(41.392659, 2.185504, 7, new DateTimeOffset(new DateTime(2018, 8, 17, 10, 45, 0), TimeSpan.FromHours(2))),
                new Marker(46.769364, 23.589673, 3, new DateTimeOffset(new DateTime(2018, 1, 1, 8, 45, 0), TimeSpan.FromHours(3)))
            };
        }
    }
}
