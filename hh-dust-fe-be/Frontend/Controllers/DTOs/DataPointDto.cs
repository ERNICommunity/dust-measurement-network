using System;

namespace Frontend.Controllers.DTOs
{
    public class DataPointDto
    {
        public DateTimeOffset Timestamp { get; set; }
        public double ParticulateMatter25 { get; set; }
        public double ParticulateMatter100 { get; set; }
    }
}