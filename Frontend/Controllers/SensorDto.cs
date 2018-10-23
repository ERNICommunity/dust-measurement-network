using System;

namespace Frontend.Controllers
{
    public class SensorDto 
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public double Latitude { get; set; }
        public double Longitude { get; set; }
        public double? ParticulateMatter25 { get; set; }
        public double? ParticulateMatter100 { get; set; }
        public DateTimeOffset? Timestamp { get; set; }
    }
}
