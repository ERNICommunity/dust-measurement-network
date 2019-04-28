package converter

import (
	"../protobuf"
	"../timeseriesdb"
)

func convertBatteryState(protoData protobuf.BatteryState) timeseriesdb.BatteryData {
	var batDataDb timeseriesdb.BatteryData

	batDataDb.Voltage = *protoData.Voltage
	batDataDb.Health = float32(*protoData.State)

	return batDataDb
}

func convertDustMeasurement(protoData protobuf.DustSensorMeasurement) timeseriesdb.DustMeasurement {
	var dustMeasurementDb timeseriesdb.DustMeasurement

	dustMeasurementDb.ParticularMatter10uM = *protoData.ParticularMatter10Um
	dustMeasurementDb.ParticularMatter25uM = *protoData.ParticularMatter2_5Um
	dustMeasurementDb.TemperatureC = *protoData.Temperature
	dustMeasurementDb.HumidityPercent = *protoData.Humidity

	return dustMeasurementDb
}
