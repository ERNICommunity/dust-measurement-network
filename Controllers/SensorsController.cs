using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Model;

namespace hh_fe.Controllers
{
    [Route("api/[controller]")]
    public class SensorsController : Controller
    {
        private readonly DustContext _ctx;

        public SensorsController(DustContext ctx)
        {
            if(ctx == null)
            {
                throw new ArgumentNullException(nameof(ctx));
            }

            _ctx = ctx;
        }

        public IEnumerable<SensorDto> Index()
        {
            return from s in _ctx.Sensors
            let lastData = s.SensorDatas.OrderByDescending(x => x.Timestamp).FirstOrDefault()
            select new SensorDto {
                Id = s.Id,
                Lat = s.Latitude,
                Lon = s.Longitude,
                Timestamp = lastData == null ? (DateTimeOffset?)null : lastData.Timestamp,
                ParticulateMatter25 = lastData == null ? (double?)null : lastData.ParticulateMatter25,
                ParticulateMatter100 = lastData == null ? (double?)null : lastData.ParticulateMatter100
            };
        }

        [Route("{id:int}/history")]
        public IEnumerable<DataPointDto> History(int id, [FromQuery]long from, [FromQuery]long to)
        {
            return _ctx.SensorDatas.Where(x => x.SensorId == id)
            .Where(x => x.Timestamp >= DateTimeOffset.FromUnixTimeMilliseconds(from))
            .Where(x => x.Timestamp < DateTimeOffset.FromUnixTimeMilliseconds(to))
            .OrderBy(x => x.Timestamp)
            .Select(x => new DataPointDto
             { 
                 Timestamp = x.Timestamp,
                 ParticulateMatter25 = x.ParticulateMatter25,
                 ParticulateMatter100 = x.ParticulateMatter100
             });
        }

        [Route("{id:int}/prediction")]
        public IEnumerable<DataPointDto> Prediction(int id)
        {
            var rand = new Random();
            return Enumerable.Range(1, 5).Select(index => new DataPointDto
             {
                Timestamp = DateTimeOffset.UtcNow.AddDays(index+1),
                ParticulateMatter25 = rand.NextDouble()*500,
                ParticulateMatter100 = rand.NextDouble()*500
            });
        }
    }
}
