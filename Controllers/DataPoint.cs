using System;

namespace hh_fe.Controllers
{
    public class DataPoint
    {
        public DataPoint(DateTimeOffset timestamp, double dust1, double dust2)
        {
            Timestamp = timestamp;
            Dust1 = dust1;
            Dust2 = dust2;
        }

        public DateTimeOffset Timestamp { get; }
        public double Dust1 { get; }
        public double Dust2 { get; }
    }
}