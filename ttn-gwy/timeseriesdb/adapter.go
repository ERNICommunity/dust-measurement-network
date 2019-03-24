package timeseriesdb

import (
	"fmt"

	"../decoder"
)

type TimeSeriesDataBaseConfig struct {
	Address        string
	Username       string
	Password       string
	DataBaseName   string
	LogFileName    string
	EnableDataBase bool
	EnableLogFile  bool
}

type timeSeriesDataBase struct {
	fileWriter             FileSettings
	dataBaseWriter         InfluxConnection
	enabledWritingDatabase bool
	enabledWritingFile     bool
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
func (r *timeSeriesDataBase) writeDustMeasurementData(chanDustMeasurementState <-chan decoder.ReceivedDustMeasurement) {
	for val := range chanDustMeasurementState {
		var head decoder.ReceivedMessageHead
		var data decoder.DustMeasurementData
		head = val.Head
		data = val.Data

		position := convertPositionGeo(head.Position)
		dustData := convertDustData(data)

		fmt.Println(val)
		if r.enabledWritingFile {
			r.fileWriter.WriteDustMeasurementEntry(head.DeviceID, dustData, position, head.Timestamp)
		}
		if r.enabledWritingDatabase {
			error := r.dataBaseWriter.WriteDustMeasurementEntry(head.DeviceID, dustData, position, head.Timestamp)
			if error != nil {
				fmt.Println(error)
			}

		}
	}
}

func (r *timeSeriesDataBase) writeBatteryState(chanBatteryState <-chan decoder.ReceivedBatteryStateData) {
	for val := range chanBatteryState {
		var head decoder.ReceivedMessageHead
		var data decoder.BatteryStateData
		head = val.Head
		data = val.Data

		position := convertPositionGeo(head.Position)
		batData := convertBatteryData(data)
		// read from a channel
		fmt.Println(val)
		if r.enabledWritingFile {
			r.fileWriter.WriteBatteryStateEntry(head.DeviceID, batData, position, head.Timestamp)
		}
		if r.enabledWritingDatabase {
			error := r.dataBaseWriter.WriteBatteryEntry(head.DeviceID, batData, position, head.Timestamp)
			if error != nil {
				fmt.Println(error)
			}

		}
	}
}

//WriteData
func WriteData(config TimeSeriesDataBaseConfig, receivedMassages decoder.ReceivedMessage) {
	var tsdb timeSeriesDataBase
	tsdb.dataBaseWriter.Address = config.Address
	tsdb.dataBaseWriter.Username = config.Username
	tsdb.dataBaseWriter.Password = config.Password
	tsdb.dataBaseWriter.DataBaseName = config.DataBaseName
	tsdb.fileWriter.FileName = config.LogFileName
	tsdb.enabledWritingDatabase = config.EnableDataBase
	tsdb.enabledWritingFile = config.EnableLogFile
	tsdb.fileWriter.Initialize()
	errorConnection := tsdb.dataBaseWriter.InitializeInfluxConnection()
	if errorConnection != nil {
		tsdb.enabledWritingDatabase = false
		fmt.Println(errorConnection)
	}
	go tsdb.writeBatteryState(receivedMassages.BatteryStateData)
	go tsdb.writeDustMeasurementData(receivedMassages.DustMeasurementData)
}
