package configurator

import (
	"github.com/micro/go-config"
)

func LoadMqttConfig() HostMqtt {
	var mqttSetting HostMqtt
	config.Get("hosts", "mqtt").Scan(&mqttSetting)
	return mqttSetting
}

func LoadTimeseriesConfig() (TimeSeries, error) {
	var timeSeriesSetting TimeSeries
	error := config.Get("hosts", "timeSeries").Scan(&timeSeriesSetting)
	return timeSeriesSetting, error
}

//StartWatchMqttChanges is experimental
func StartWatchMqttChanges() <-chan HostMqtt {
	chanMQTT := make(chan HostMqtt)
	go func() {

		for {
			w, err := config.Watch("hosts", "mqtt")
			if err != nil {
				continue
			}

			// wait for next value
			v, err := w.Next()
			if err != nil {
				continue
			}

			var mqttSettigns HostMqtt

			erro := v.Scan(&mqttSettigns)
			if erro == nil {
				chanMQTT <- mqttSettigns
			}
		}
	}()
	return chanMQTT
}
