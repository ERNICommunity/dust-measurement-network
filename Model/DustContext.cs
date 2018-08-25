using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;

namespace Model
{
    public class DustContext : DbContext
    {
         public DustContext(DbContextOptions<DustContext> options)
            : base(options)
        {
        }

        public DbSet<Sensor> Sensors { get; set; }
        public DbSet<SensorData> SensorDatas { get; set; }
        public DbSet<DataEnhancement> DataEnhancements { get; set; }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Sensor>().HasKey(x => x.Id);
            modelBuilder.Entity<Sensor>().HasMany(x => x.SensorDatas).WithOne(x => x.Sensor).HasForeignKey(x => x.SensorId).OnDelete(DeleteBehavior.Restrict);
            modelBuilder.Entity<SensorData>().HasKey(x => x.Id);
            modelBuilder.Entity<SensorData>().HasOne(x => x.DataEnhancement).WithOne(x => x.SensorData).HasForeignKey<DataEnhancement>(x => x.SensorDataId).OnDelete(DeleteBehavior.Restrict);

            // dummy data
            var rand = new Random();

            modelBuilder.Entity<Sensor>().HasData(
                new Sensor { Id = 1, Latitude = 48.142402, Longitude = 17.108669, Altitude = 500 },
                new Sensor { Id = 2, Latitude = 47.008323, Longitude = 8.486309, Altitude = 350 },
                new Sensor { Id = 3, Latitude = 14.598882, Longitude = 120.971040, Altitude = 220 },
                new Sensor { Id = 4, Latitude = 47.370677,Longitude = 8.540103, Altitude = 110 },
                new Sensor { Id = 5, Latitude = 46.769364, Longitude = 23.589673, Altitude = 30 }
            );

            modelBuilder.Entity<SensorData>().HasData(
                new SensorData { Id = 1, SensorId = 1, Timestamp = DateTimeOffset.UtcNow, AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*500, ParticulateMatter100 = rand.NextDouble()*500 },
                new SensorData { Id = 2, SensorId = 1, Timestamp = DateTimeOffset.UtcNow.AddHours(-1), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*500, ParticulateMatter100 = rand.NextDouble()*500 },
                new SensorData { Id = 3, SensorId = 1, Timestamp = DateTimeOffset.UtcNow.AddHours(-2), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*500, ParticulateMatter100 = rand.NextDouble()*500 },
                new SensorData { Id = 4, SensorId = 1, Timestamp = DateTimeOffset.UtcNow.AddHours(-3), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*500, ParticulateMatter100 = rand.NextDouble()*500 },
                new SensorData { Id = 5, SensorId = 2, Timestamp = DateTimeOffset.UtcNow, AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*500, ParticulateMatter100 = rand.NextDouble()*500 },
                new SensorData { Id = 6, SensorId = 2, Timestamp = DateTimeOffset.UtcNow.AddHours(-1), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*500, ParticulateMatter100 = rand.NextDouble()*500 },
                new SensorData { Id = 7, SensorId = 2, Timestamp = DateTimeOffset.UtcNow.AddHours(-2), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*500, ParticulateMatter100 = rand.NextDouble()*500 },
                 new SensorData { Id = 8, SensorId = 3, Timestamp = DateTimeOffset.UtcNow, AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*500, ParticulateMatter100 = rand.NextDouble()*500 },
                new SensorData { Id = 9, SensorId = 3, Timestamp = DateTimeOffset.UtcNow.AddHours(-1), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*500, ParticulateMatter100 = rand.NextDouble()*500 },
                 new SensorData { Id = 10, SensorId = 4, Timestamp = DateTimeOffset.UtcNow, AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*500, ParticulateMatter100 = rand.NextDouble()*500 },
                new SensorData { Id = 11, SensorId = 4, Timestamp = DateTimeOffset.UtcNow.AddHours(-1), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*500, ParticulateMatter100 = rand.NextDouble()*500 },
                new SensorData { Id = 12, SensorId = 4, Timestamp = DateTimeOffset.UtcNow.AddHours(-2), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*500, ParticulateMatter100 = rand.NextDouble()*500 },
                new SensorData { Id = 13, SensorId = 4, Timestamp = DateTimeOffset.UtcNow.AddHours(-3), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*500, ParticulateMatter100 = rand.NextDouble()*500 }
            );

       }
    }
}