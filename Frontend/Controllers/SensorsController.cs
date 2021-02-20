using System;
using System.Collections.Generic;
using System.Linq;
using Frontend.Controllers.DTOs;
using Microsoft.AspNetCore.Mvc;
using Model;
using Microsoft.EntityFrameworkCore;
using System.Threading.Tasks;

namespace Frontend.Controllers
{
    [Route("api/[controller]")]
    public class SensorsController : ControllerBase
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

        public async Task<IEnumerable<SensorDto>> Index([FromQuery]double minLon, [FromQuery]double minLat, [FromQuery]double maxLon, [FromQuery]double maxLat)
        {
            return await (from s in _ctx.Sensors
            where minLon < maxLon ? (s.Longitude >= minLon && s.Longitude <= maxLon) : ((s.Longitude >= minLon && s.Longitude <= 180) || (s.Longitude >= -180 && s.Longitude <= maxLon)) 
            where s.Latitude >= minLat && s.Latitude <= maxLat 
            let lastData = s.SensorDatas.OrderByDescending(x => x.Timestamp).FirstOrDefault()
            select new SensorDto {
                Id = s.Id,
                Name = s.Name,
                Latitude = s.Latitude,
                Longitude = s.Longitude,
                Timestamp = lastData == null ? (DateTimeOffset?)null : lastData.Timestamp,
                ParticulateMatter25 = lastData == null ? (double?)null : lastData.ParticulateMatter25,
                ParticulateMatter100 = lastData == null ? (double?)null : lastData.ParticulateMatter100
            }).ToArrayAsync();
        }

        [Route("{id:int}/history")]
        public async Task<IEnumerable<DataPointDto>> History(int id, [FromQuery]long from, [FromQuery]long to)
        {
            return await _ctx.SensorDatas.Where(x => x.SensorId == id)
            .Where(x => x.Timestamp >= DateTimeOffset.FromUnixTimeMilliseconds(from))
            .Where(x => x.Timestamp < DateTimeOffset.FromUnixTimeMilliseconds(to))
            .OrderBy(x => x.Timestamp)
            .Select(x => new DataPointDto
             { 
                 Timestamp = x.Timestamp,
                 ParticulateMatter25 = x.ParticulateMatter25,
                 ParticulateMatter100 = x.ParticulateMatter100
             }).ToArrayAsync();
        }

        [Route("{id:int}/prediction")]
        public IEnumerable<DataPointDto> Prediction(int id)
        {
            var rand = new Random();
            return Enumerable.Range(1, 10).Select(index => new DataPointDto
             {
                Timestamp = DateTimeOffset.UtcNow.AddDays(index+1),
                ParticulateMatter25 = rand.NextDouble()*60,
                ParticulateMatter100 = rand.NextDouble()*100
            }).ToArray();
        }
    }
}
