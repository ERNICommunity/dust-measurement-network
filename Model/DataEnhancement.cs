namespace Model
{
    public class DataEnhancement
    {
        public int Id { get; set; }
        public double? WindDirection { get; set; }
        public double WindSpeed { get; set; }
        public double Pressure { get; set; }
        public int SensorDataId { get; set; }
        public SensorData SensorData { get; set; }
    }
}