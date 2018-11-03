using System;
using System.IO;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;
using Model;
using Npgsql;

namespace DataSyncService
{
    class Program
    {
        static void Main(string[] args)
        {
            var builder = new ConfigurationBuilder()
                .SetBasePath(Directory.GetCurrentDirectory())
                .AddJsonFile("appsettings.json", optional: true, reloadOnChange: true)
                .AddUserSecrets<Program>()
                .AddEnvironmentVariables();           
            var configuration = builder.Build();

            var csBuilder = new NpgsqlConnectionStringBuilder(configuration.GetConnectionString("DustDatabase"));
            csBuilder.ApplicationName = "DMN DataSyncService";

            var optionsBuilder = new DbContextOptionsBuilder<DustContext>();
            optionsBuilder.UseNpgsql(csBuilder.ToString());

            using(var ctx = new DustContext(optionsBuilder.Options))
            {
                foreach(var sensor in ctx.Sensors)
                {
                    Console.WriteLine($"Sensor ({sensor.Id}): {sensor.Name}@[{sensor.Latitude}, {sensor.Longitude}]");
                }
            }
        }
    }
}
