using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;

namespace hh_fe.Controllers
{
    [Route("api/[controller]")]
    public class SensorsController : Controller
    {
        private readonly Random _rand = new Random();

        public IEnumerable<Sensor> Index()
        {
            return new[] {
                new Sensor(1, 47.0027524, 8.4939782, 5.5, 6.2, new DateTimeOffset(new DateTime(2018, 8, 20, 12, 0, 0), TimeSpan.FromHours(2))),
                new Sensor(2, 47.008323, 8.486309, 2.4, 7.3, new DateTimeOffset(new DateTime(2018, 7, 15, 1, 0, 0), TimeSpan.FromHours(1))),
                new Sensor(3, 14.598882, 120.971040, 10.2, 1.4, new DateTimeOffset(new DateTime(2018, 5, 1, 22, 30, 0), TimeSpan.FromHours(8))),
                new Sensor(4, 47.370677, 8.540103, 4.4, 60.5, new DateTimeOffset(new DateTime(2018, 8, 22, 15, 30, 0), TimeSpan.FromHours(1))),
                new Sensor(5, 541.392659, 2.185504, 7.4, 9.3, new DateTimeOffset(new DateTime(2018, 8, 17, 10, 45, 0), TimeSpan.FromHours(2))),
                new Sensor(6, 46.769364, 23.589673, 3.0, 4.0, new DateTimeOffset(new DateTime(2018, 1, 1, 8, 45, 0), TimeSpan.FromHours(3)))
            };
        }

        [Route("{id:int}/history")]
        public IEnumerable<DataPoint> History(int id)
        {
            return Enumerable.Range(1, 10).Select(index => new DataPoint(
                DateTimeOffset.UtcNow.AddMinutes(-index),
                 _rand.NextDouble()*100,
                 _rand.NextDouble()*100));
        }

        [Route("{id:int}/prediction")]
        public IEnumerable<DataPoint> Prediction(int id)
        {
            return Enumerable.Range(1, 5).Select(index => new DataPoint(
                DateTimeOffset.UtcNow.AddDays(index+1),
                 _rand.NextDouble()*100,
                 _rand.NextDouble()*100));
        }
    }
}
