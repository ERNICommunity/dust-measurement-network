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
            modelBuilder.Entity<Sensor>().Property(x => x.Name).IsRequired();
            modelBuilder.Entity<Sensor>().HasIndex(x => x.NodeId).IsUnique();
            modelBuilder.Entity<Sensor>().HasMany(x => x.SensorDatas).WithOne(x => x.Sensor).HasForeignKey(x => x.SensorId).OnDelete(DeleteBehavior.Restrict);
            modelBuilder.Entity<SensorData>().HasKey(x => x.Id);
            modelBuilder.Entity<SensorData>().HasOne(x => x.DataEnhancement).WithOne(x => x.SensorData).HasForeignKey<DataEnhancement>(x => x.SensorDataId).OnDelete(DeleteBehavior.Restrict);

            // dummy data
            var rand = new Random();

            modelBuilder.Entity<Sensor>().HasData(
                new Sensor { Id = 1, Name = "Bratislava", Latitude = 48.142402, Longitude = 17.108669, Altitude = 500 },
                new Sensor { Id = 2, Name = "Luzern", Latitude = 47.052902, Longitude = 8.306247, Altitude = 350 },
                new Sensor { Id = 3, Name = "Barcelona", Latitude = 41.393955, Longitude = 2.176578, Altitude = 220 },
                new Sensor { Id = 4, Name = "Zurich", Latitude = 47.370677,Longitude = 8.540103, Altitude = 110 },
                new Sensor { Id = 5, Name = "Cluj-Napoca", Latitude = 46.769364, Longitude = 23.589673, Altitude = 30 },
                new Sensor { Id = 6, Name = "Manila", Latitude = 14.596185, Longitude = 120.982680, Altitude = 30 },
                new Sensor { Id = 7, Name = "Frankfurt", Latitude = 50.103355, Longitude = 8.662782, Altitude = 250 },
                new Sensor { Id = 8, Name = "Madrid", Latitude = 40.417938, Longitude = -3.702423, Altitude = 150 },
                new Sensor { Id = 9, Name = "Singapore", Latitude = 1.302524, Longitude = 103.861865, Altitude = 65 },
                new Sensor { Id = 10, Name = "Chopok", Latitude = 48.926524, Longitude = 19.602916, Altitude = 65 }
            );

            modelBuilder.Entity<SensorData>().HasData(
                new SensorData { Id = 1, SensorId = 1, Timestamp = DateTimeOffset.UtcNow, AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*500, ParticulateMatter100 = rand.NextDouble()*500 },
                new SensorData { Id = 2, SensorId = 1, Timestamp = DateTimeOffset.UtcNow.AddHours(-1), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*200, ParticulateMatter100 = rand.NextDouble()*200 },
                new SensorData { Id = 3, SensorId = 1, Timestamp = DateTimeOffset.UtcNow.AddHours(-2), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*200, ParticulateMatter100 = rand.NextDouble()*200 },
                new SensorData { Id = 4, SensorId = 1, Timestamp = DateTimeOffset.UtcNow.AddHours(-3), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*200, ParticulateMatter100 = rand.NextDouble()*200 },
                new SensorData { Id = 5, SensorId = 2, Timestamp = DateTimeOffset.UtcNow, AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*500, ParticulateMatter100 = rand.NextDouble()*200 },
                new SensorData { Id = 6, SensorId = 2, Timestamp = DateTimeOffset.UtcNow.AddHours(-1), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*200, ParticulateMatter100 = rand.NextDouble()*200 },
                new SensorData { Id = 7, SensorId = 2, Timestamp = DateTimeOffset.UtcNow.AddHours(-2), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*200, ParticulateMatter100 = rand.NextDouble()*200 },
                 new SensorData { Id = 8, SensorId = 3, Timestamp = DateTimeOffset.UtcNow, AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*500, ParticulateMatter100 = rand.NextDouble()*200 },
                new SensorData { Id = 9, SensorId = 3, Timestamp = DateTimeOffset.UtcNow.AddHours(-1), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*200, ParticulateMatter100 = rand.NextDouble()*200 },
                 new SensorData { Id = 10, SensorId = 4, Timestamp = DateTimeOffset.UtcNow, AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*500, ParticulateMatter100 = rand.NextDouble()*200 },
                new SensorData { Id = 11, SensorId = 4, Timestamp = DateTimeOffset.UtcNow.AddHours(-1), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*200, ParticulateMatter100 = rand.NextDouble()*200 },
                new SensorData { Id = 12, SensorId = 4, Timestamp = DateTimeOffset.UtcNow.AddHours(-2), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*200, ParticulateMatter100 = rand.NextDouble()*200 },
                new SensorData { Id = 13, SensorId = 4, Timestamp = DateTimeOffset.UtcNow.AddHours(-3), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*200, ParticulateMatter100 = rand.NextDouble()*200 },
                new SensorData { Id = 14, SensorId = 10, Timestamp = DateTimeOffset.UtcNow.AddHours(-3), AirTemperature = -10 + rand.NextDouble()*50, RelativeHumidity = rand.NextDouble()*100, ParticulateMatter25 = rand.NextDouble()*200, ParticulateMatter100 = rand.NextDouble()*200 }
            );

       }
    }
}