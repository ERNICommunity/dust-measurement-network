using System;

namespace hh_fe.Controllers
{
    public class Sensor 
    {
        public Sensor(int id, double lat, double lon, double dust1, double dust2, DateTimeOffset timestamp)
        {
            Id = id;
            Lat = lat;
            Lon = lon;
            Dust1 = dust1;
            Dust2 = dust2;
            Timestamp = timestamp;
        }
        public int Id { get; }
        public double Lat { get; }
        public double Lon { get; }
        public double Dust1 { get; }
        public double Dust2 { get; }
        public DateTimeOffset Timestamp { get; }
    }
}
