using System;

namespace hh_fe.Controllers
{
    public class SensorDto 
    {
        public int Id { get; set; }
        public double Lat { get; set; }
        public double Lon { get; set; }
        public double? ParticulateMatter25 { get; set; }
        public double? ParticulateMatter100 { get; set; }
        public DateTimeOffset? Timestamp { get; set; }
    }
}
