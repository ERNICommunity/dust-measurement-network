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
            modelBuilder.Entity<Sensor>().Property(x => x.NodeId).IsRequired();
            modelBuilder.Entity<Sensor>().HasIndex(x => x.NodeId).IsUnique();
            modelBuilder.Entity<Sensor>().HasMany(x => x.SensorDatas).WithOne(x => x.Sensor).HasForeignKey(x => x.SensorId).OnDelete(DeleteBehavior.Restrict);
            modelBuilder.Entity<SensorData>().HasKey(x => x.Id);
            modelBuilder.Entity<SensorData>().HasOne(x => x.DataEnhancement).WithOne(x => x.SensorData).HasForeignKey<DataEnhancement>(x => x.SensorDataId).OnDelete(DeleteBehavior.Restrict);
            modelBuilder.Entity<DataEnhancement>().HasKey(x => x.SensorDataId);
       }
    }
}