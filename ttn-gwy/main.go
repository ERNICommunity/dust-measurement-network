package main

import (
	"fmt"
	"runtime"

	"./configurator"
	"./decoder"
	"./timeseriesdb"
	"./ttnconnector"
)

var mqttConfig ttnconnector.MqttClientConfiguration
var timeSeriesDBConfig timeseriesdb.TimeSeriesDataBaseConfig

func Init() error {
	var appConfig configurator.Configurator
	error := appConfig.Init("config/config.json")
	if error != nil {
		fmt.Println(error)
		return error
	}
	mqttTTNConfig := appConfig.GetMqttConfig()
	mqttConfig.Id = mqttTTNConfig.ID
	mqttConfig.UserName = mqttTTNConfig.Username
	mqttConfig.Password = mqttTTNConfig.Password
	mqttConfig.Brokers = mqttTTNConfig.Broker

	timeSeriesConfig := appConfig.GetTimeseriesConfig()
	timeSeriesDBConfig.Address = timeSeriesConfig.DataBase.Address
	timeSeriesDBConfig.Username = timeSeriesConfig.DataBase.Username
	timeSeriesDBConfig.Password = timeSeriesConfig.DataBase.Password
	timeSeriesDBConfig.DataBaseName = timeSeriesConfig.DataBase.Name
	timeSeriesDBConfig.LogFileName = timeSeriesConfig.LogFile.Path
	timeSeriesDBConfig.EnableDataBase = timeSeriesConfig.DataBase.Enable
	timeSeriesDBConfig.EnableLogFile = timeSeriesConfig.LogFile.Enable
	return error
}

func main() {
	error := Init()
	if error == nil {
		var ttnConnection ttnconnector.TTNConnection
		ttnMessages, error := ttnConnection.Subscribe(mqttConfig, "erni-dmn")
		//defer ttnConnection.Unsubscribe()
		if error != nil {
			fmt.Println(error)
		}
		decodedMessages := decoder.Decode(ttnMessages)
		timeseriesdb.WriteData(timeSeriesDBConfig, decodedMessages)
		runtime.Goexit()
	}

}
