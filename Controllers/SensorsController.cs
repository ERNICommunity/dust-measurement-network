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
        private readonly Random _rand = new Random();
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
        public IEnumerable<DataPointDto> History(int id)
        {
            return Enumerable.Range(1, 10).Select(index => new DataPointDto(
                DateTimeOffset.UtcNow.AddMinutes(-index),
                 _rand.NextDouble()*100,
                 _rand.NextDouble()*100));
        }

        [Route("{id:int}/prediction")]
        public IEnumerable<DataPointDto> Prediction(int id)
        {
            return Enumerable.Range(1, 5).Select(index => new DataPointDto(
                DateTimeOffset.UtcNow.AddDays(index+1),
                 _rand.NextDouble()*100,
                 _rand.NextDouble()*100));
        }
    }
}
