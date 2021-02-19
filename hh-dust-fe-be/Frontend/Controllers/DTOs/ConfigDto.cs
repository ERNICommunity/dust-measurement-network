using System;

namespace Frontend.Controllers.DTOs
{
    public class ConfigDto
    {
        public DateTimeOffset ServerTime { get; set; }
        public int AutorefreshIntervalMilisec { get; set; }
    }
}