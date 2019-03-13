package timeseriesdb

import (
	"fmt"

	"../decoder"
)

type TimeSeriesDataBase struct {
	FileWriter FileSettings
}

func convertPositionGeo(inputPosition decoder.PositionGeo) PositionGeo {
	var position PositionGeo
	position.Altitude = inputPosition.Altitude
	position.Latitude = inputPosition.Latitude
	position.Longitude = inputPosition.Longitude
	return position
}

func convertDustData(inputData decoder.DustMeasurementData) DustMeasurement {
	var dustData DustMeasurement
	dustData.HumidityPercent = inputData.HumidityPercent
	dustData.ParticularMatter10uM = inputData.ParticularMatter10uM
	dustData.ParticularMatter25uM = inputData.ParticularMatter25uM
	dustData.TemperatureC = inputData.TemperatureC
	return dustData
}

func convertBatteryData(inputBatteryData decoder.BatteryStateData) BatteryData {
	var batteryData BatteryData
	batteryData.Voltage = inputBatteryData.Voltage
	batteryData.Health = inputBatteryData.Health
	return batteryData
}
func (r *TimeSeriesDataBase) writeDustMeasurementData(chanDustMeasurementState <-chan decoder.ReceivedDustMeasurement) {
	for val := range chanDustMeasurementState {
		var head decoder.ReceivedMessageHead
		var data decoder.DustMeasurementData
		head = val.Head
		data = val.Data

		position := convertPositionGeo(head.Position)
		dustData := convertDustData(data)

		// read from a channel
		fmt.Println(val)
		r.FileWriter.WriteDustMeasurementEntry(head.DeviceID, dustData, position, head.Timestamp)
	}
}

func (r *TimeSeriesDataBase) writeBatteryState(chanBatteryState <-chan decoder.ReceivedBatteryStateData) {
	for val := range chanBatteryState {
		var head decoder.ReceivedMessageHead
		var data decoder.BatteryStateData
		head = val.Head
		data = val.Data

		position := convertPositionGeo(head.Position)
		batData := convertBatteryData(data)
		// read from a channel
		fmt.Println(val)
		r.FileWriter.WriteBatteryStateEntry(head.DeviceID, batData, position, head.Timestamp)
	}
}

//WriteData
func (r *TimeSeriesDataBase) WriteData(receivedMassages decoder.ReceivedMessage) {
	r.FileWriter.Initialize()
	go r.writeBatteryState(receivedMassages.BatteryStateData)
	go r.writeDustMeasurementData(receivedMassages.DustMeasurementData)
}
