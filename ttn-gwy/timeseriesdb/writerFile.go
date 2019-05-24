package timeseriesdb

import (
	"os"
	"strconv"
	"time"
)

type FileSettings struct {
	FileName string
	file     *os.File
}

func (r *FileSettings) Initialize() {
	var err error
	r.file, err = os.Create(r.FileName)
	if err != nil {
		r.file = nil
	}
}

func (r *FileSettings) WriteDustMeasurementEntry(
	devID string,
	dustMeasure DustMeasurement,
	positionGeo PositionGeo,
	timestamp time.Time,
) error {
	_, err := r.file.WriteString("dust_measurement,sensor_id=" + devID +
		" pm10=" + strconv.FormatFloat(float64(dustMeasure.ParticularMatter10uM), 'f', 6, 32) +
		",pm25=" + strconv.FormatFloat(float64(dustMeasure.ParticularMatter25uM), 'f', 1, 32) +
		",temp=" + strconv.FormatFloat(float64(dustMeasure.TemperatureC), 'f', 1, 32) +
		",humidity=" + strconv.FormatFloat(float64(dustMeasure.HumidityPercent), 'f', 1, 32) +
		",latitude=" + strconv.FormatFloat(float64(positionGeo.Latitude), 'f', 1, 32) +
		",longtitude=" + strconv.FormatFloat(float64(positionGeo.Longitude), 'f', 1, 32) +
		",altitude=" + strconv.FormatFloat(float64(positionGeo.Altitude), 'f', 1, 32) +
		" " + strconv.FormatInt(timestamp.UTC().UnixNano(), 10) +
		"\n")
	return err
}

func (r *FileSettings) WriteBatteryStateEntry(
	devID string,
	batteryData BatteryData,
	positionGeo PositionGeo,
	timestamp time.Time,
) error {
	_, err := r.file.WriteString("dust_measurement,sensor_id=" + devID +
		" voltage=" + strconv.FormatFloat(float64(batteryData.Voltage), 'f', 6, 32) +
		",health_state=" + strconv.FormatFloat(float64(batteryData.Health), 'f', 1, 32) +
		",latitude=" + strconv.FormatFloat(float64(positionGeo.Latitude), 'f', 1, 32) +
		",longtitude=" + strconv.FormatFloat(float64(positionGeo.Longitude), 'f', 1, 32) +
		",altitude=" + strconv.FormatFloat(float64(positionGeo.Altitude), 'f', 1, 32) +
		" " + strconv.FormatInt(timestamp.UTC().UnixNano(), 10) +
		"\n")
	return err
}
