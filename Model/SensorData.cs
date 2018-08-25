using System;

namespace Model
{
    public class SensorData
    {
        public int Id { get; set; }
        public DateTimeOffset Timestamp { get; set; }
        public double ParticulateMatter25 { get; set; }
        public double ParticulateMatter100 { get; set; }
        public double AirTemperature { get; set; }
        public double RelativeHumidity { get; set; }
        public int SensorId { get; set; }
        public Sensor Sensor { get; set; }
        public int? DataEnhancemetId { get; set; }
        public DataEnhancement DataEnhancement { get; set; }         
    }
}