package timeseriesdb

import (
	"errors"
	"fmt"
	"time"

	"github.com/influxdata/influxdb/tree/1.7/client/v2"
)

type InfluxConnection struct {
	Address      string
	Username     string
	Password     string
	DataBaseName string
	client       *client.Client
}

func (r *InfluxConnection) InitializeInfluxConnection() error {
	c, err := client.NewHTTPClient(client.HTTPConfig{
		Addr:     r.Address,
		Username: r.Username,
		Password: r.Password,
	})
	fmt.Println("CONNECT " + r.Address)
	if err != nil {
		fmt.Println(err)
		r.client = nil
		return errors.New("Client not connected")
	}
	r.client = &c
	return nil
}

func (r *InfluxConnection) writeToInfluxDB(
	dataBase string,
	point string,
	tags map[string]string,
	fields map[string]interface{},
	timestamp time.Time,
) error {
	// Create a new point batch
	bp, err := client.NewBatchPoints(client.BatchPointsConfig{
		Database:  dataBase,
		Precision: "s",
	})

	if err != nil {
		return err
	}
	// Create a new point and add to batch
	pt, err := client.NewPoint(point, tags, fields, timestamp)
	if err != nil {
		return err
	}
	bp.AddPoint(pt)

	err = (*r.client).Write(bp)

	return err

}

/*WriteDustMeasurementEntry Entry a dust Measurement Data to DataBase */
func (r *InfluxConnection) WriteDustMeasurementEntry(
	devID string,
	dustMeasure DustMeasurement,
	positionGeo PositionGeo,
	timestamp time.Time,
) error {
	// Create a point and add to batch
	tags := map[string]string{"sensor_id": devID}
	fields := map[string]interface{}{
		"pm10":      dustMeasure.ParticularMatter10uM,
		"pm25":      dustMeasure.ParticularMatter25uM,
		"temp":      dustMeasure.TemperatureC,
		"humidity":  dustMeasure.HumidityPercent,
		"latitude":  positionGeo.Latitude,
		"longitude": positionGeo.Longitude,
		"altitude":  positionGeo.Altitude,
	}
	return r.writeToInfluxDB(r.DataBaseName, "dust_measurement", tags, fields, timestamp)
}

/*WriteBatteryEntry Entry a dust Measurement Data to DataBase */
func (r *InfluxConnection) WriteBatteryEntry(
	devID string,
	batteryData BatteryData,
	positionGeo PositionGeo,
	timestamp time.Time,
) error {
	// Create a point and add to batch
	tags := map[string]string{"sensor_id": devID}
	fields := map[string]interface{}{
		"voltage":      batteryData.Voltage,
		"health_state": batteryData.Health,
		"latitude":     positionGeo.Latitude,
		"longitude":    positionGeo.Longitude,
		"altitude":     positionGeo.Altitude,
	}
	return r.writeToInfluxDB(r.DataBaseName, "sensor_state", tags, fields, timestamp)
}
