using System;

namespace hh_fe.Controllers
{
    public partial class SampleDataController
    {
        public class Marker 
        {
            public Marker(double lat, double lon, int size, DateTimeOffset timestamp)
            {
                Lat = lat;
                Lon = lon;
                Size = size;
                Timestamp = timestamp;
            }

            public double Lat { get; }
            public double Lon { get; }
            public int Size { get; }
            public DateTimeOffset Timestamp { get; }
        }
    }
}
