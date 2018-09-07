using System.Collections.Generic;

namespace Model
{
    public class Sensor
    {
        public int Id { get; set; }
        public string NodeId { get; set; }
        public string Name { get; set; }
        public double Longitude { get; set; }
        public double Latitude { get; set; }
        public double Altitude { get; set; }
        public ICollection<SensorData> SensorDatas { get; set; }
    }
}