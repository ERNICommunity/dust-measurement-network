package configurator

import (
	"github.com/micro/go-config"
)

type Configurator struct {
	mqttSetting       HostMqtt
	timeSeriesSetting TimeSeries
}

func (r *Configurator) Init(configFile string) error {
	error := config.LoadFile(configFile)
	if error != nil {
		return error
	}
	error = config.Get("hosts", "mqtt").Scan(&r.mqttSetting)
	if error != nil {
		return error
	}
	error = config.Get("hosts", "timeSeries").Scan(&r.timeSeriesSetting)
	if error != nil {
		return error
	}
	return error
}

func (r *Configurator) GetMqttConfig() HostMqtt {
	return r.mqttSetting
}

func (r *Configurator) GetTimeseriesConfig() TimeSeries {
	return r.timeSeriesSetting
}
