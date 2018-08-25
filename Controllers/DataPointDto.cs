using System;

namespace hh_fe.Controllers
{
    public class DataPointDto
    {
        public DateTimeOffset Timestamp { get; set; }
        public double ParticulateMatter25 { get; set; }
        public double ParticulateMatter100 { get; set; }
    }
}