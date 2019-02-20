package decoder

import "time"

type PositionGeo struct {
	Latitude  float32
	Longitude float32
	Altitude  float32
}

type DustMeasurementData struct {
	ParticularMatter10uM float32
	ParticularMatter25uM float32
	TemperatureC         float32
	HumidityPercent      float32
}

type BatteryStateData struct {
	Voltage float32
	Health  float32
}

type ReceivedMessageHead struct {
	DeviceID      string
	ApplicationID string
	Timestamp     time.Time
	AirTime       time.Duration
	Position      PositionGeo
}

type ReceivedDustMeasurement struct {
	Head ReceivedMessageHead
	Data DustMeasurementData
}

type ReceivedBatteryStateData struct {
	Head ReceivedMessageHead
	Data BatteryStateData
}
