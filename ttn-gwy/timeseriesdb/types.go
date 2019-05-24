package timeseriesdb

type PositionGeo struct {
	Latitude  float32
	Longitude float32
	Altitude  float32
}

type DustMeasurement struct {
	ParticularMatter10uM float32
	ParticularMatter25uM float32
	TemperatureC         float32
	HumidityPercent      float32
}

type BatteryData struct {
	Voltage float32
	Health  float32
}
