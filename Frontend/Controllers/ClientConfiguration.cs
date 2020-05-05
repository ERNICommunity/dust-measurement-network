using System;
using Frontend.Controllers.DTOs;
using Microsoft.AspNetCore.Mvc;
using Model;

namespace Frontend.Controllers
{
    [Route("api/[controller]")]
    public class ClientConfigurationController : ControllerBase
    {
        private readonly DustContext _ctx;

        public ClientConfigurationController(DustContext ctx)
        {
            if(ctx == null)
            {
                throw new ArgumentNullException(nameof(ctx));
            }

            _ctx = ctx;
        }

        public ConfigDto Index([FromQuery]double minLon, [FromQuery]double minLat, [FromQuery]double maxLon, [FromQuery]double maxLat)
        {
            return new ConfigDto 
            {
                ServerTime = DateTimeOffset.Now,
                AutorefreshIntervalMilisec = 5000
            };
        }
    }
}
