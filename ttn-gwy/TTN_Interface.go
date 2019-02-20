package main

import (
	"encoding/binary"
	"errors"
	"fmt"
	"os"
	"runtime"
	"strconv"
	"time"

	"./decoder"
	"./protobuf"
	"./ttnconnector"
	"github.com/TheThingsNetwork/ttn/core/types"
	"github.com/gogo/protobuf/proto"
	"github.com/influxdata/influxdb/client/v2"
)

const (
	MyDB     = "square_holes"
	username = "bubba"
	password = "bumblebeetuna"
)

var (
	influxClient *client.Client
)

type MqttConfiguration struct {
	ApplicationId  string
	ApplicationKey string
	broker         string
}

func checkError(err error) {
	if err != nil {
		fmt.Fprintf(os.Stderr, "Fatal error: %s", err.Error())
		runtime.Goexit()
	}
}

func decodeBatteryState(devId string,
	latitude float32,
	longitude float32,
	altitude int32,
	timestamp types.JSONTime,
	data *protobuf.BatteryState, file *os.File,
	influxClient *client.Client) {
	voltage := data.Voltage
	stringTime, err := timestamp.MarshalText()
	checkError(err)
	t, err := time.Parse(time.RFC3339, string(stringTime[:]))
	checkError(err)
	file.WriteString("sensor_state,sensor_id=" + devId +
		" voltage=" + strconv.FormatFloat(float64(*voltage), 'f', 6, 32) +
		",health_state=" + strconv.FormatFloat(float64(*data.State), 'f', 1, 32) +
		",latitude=" + strconv.FormatFloat(float64(latitude), 'f', 1, 32) +
		",longtitude=" + strconv.FormatFloat(float64(longitude), 'f', 1, 32) +
		",altitude=" + strconv.FormatFloat(float64(altitude), 'f', 1, 32) +
		" " + strconv.FormatInt(t.UTC().UnixNano(), 10) +
		" \n")
	writeToInfluxDbBatteryData(influxClient, devId, *voltage, float32(*data.State), latitude, longitude, float32(altitude), t)
}

func initializeInfluxConnection() (*client.Client, error) {
	// Create a new HTTPClient
	c, err := client.NewHTTPClient(client.HTTPConfig{
		Addr:     "http://104.40.196.109:8086",
		Username: "admin",
		Password: "hnh2018",
	})
	if err != nil {
		fmt.Println(err)
		return nil, errors.New("Client not connected")
	}
	return &c, nil
}

func writeToInfluxDbBatteryData(influxClient *client.Client,
	devId string,
	voltage float32,
	health float32,
	lat float32,
	long float32,
	alt float32,
	timestamp time.Time,
) error {
	// Create a new point batch
	bp, err := client.NewBatchPoints(client.BatchPointsConfig{
		Database:  "dmn",
		Precision: "s",
	})
	if err != nil {
		fmt.Println(err)
	}

	// Create a point and add to batch
	tags := map[string]string{"sensor_id": devId}
	fields := map[string]interface{}{
		"voltage":      voltage, //*protobuf.ParticularMatter10um,
		"health_state": health,
		"latitude":     lat,
		"longitude":    long,
		"altitude":     alt,
	}

	pt, err := client.NewPoint("sensor_state", tags, fields, timestamp)
	if err != nil {
		fmt.Println(err)
		return err
	}
	bp.AddPoint(pt)

	// Write the batch
	fmt.Println(influxClient)
	if err := (*influxClient).Write(bp); err != nil {
		fmt.Println(err)
	}
	return nil
}

func writeToInfluxDbDustData(influxClient *client.Client,
	devId string,
	pm10 float32,
	pm25 float32,
	temp float32,
	humidity float32,
	lat float32,
	long float32,
	alt float32,
	timestamp time.Time,
) error {
	// Create a new point batch
	bp, err := client.NewBatchPoints(client.BatchPointsConfig{
		Database:  "dmn",
		Precision: "s",
	})
	if err != nil {
		fmt.Println(err)
	}

	// Create a point and add to batch
	tags := map[string]string{"sensor_id": devId}
	fields := map[string]interface{}{
		"pm10":      pm10, //*protobuf.ParticularMatter10um,
		"pm25":      pm25,
		"temp":      temp,
		"humidity":  humidity,
		"latitude":  lat,
		"longitude": long,
		"altitude":  alt,
	}

	pt, err := client.NewPoint("dust_measurement", tags, fields, timestamp)
	if err != nil {
		fmt.Println(err)
		return err
	}
	bp.AddPoint(pt)

	// Write the batch
	fmt.Println(influxClient)
	if err := (*influxClient).Write(bp); err != nil {
		fmt.Println(err)
	}
	return nil
}

func decodeDustMeasurement(devId string,
	latitude float32,
	longitude float32,
	altitude int32,
	timestamp types.JSONTime,
	data *protobuf.DustSensorMeasurement, file *os.File,
	influxClient *client.Client) {
	stringTime, err := timestamp.MarshalText()
	checkError(err)
	t, err := time.Parse(time.RFC3339, string(stringTime[:]))
	checkError(err)
	file.WriteString("dust_measurement,sensor_id=" + devId +
		" pm10=" + strconv.FormatFloat(float64(*data.ParticularMatter10Um), 'f', 6, 32) +
		",pm25=" + strconv.FormatFloat(float64(*data.ParticularMatter2_5Um), 'f', 1, 32) +
		",temp=" + strconv.FormatFloat(float64(*data.Temperature), 'f', 1, 32) +
		",humidity=" + strconv.FormatFloat(float64(*data.Humidity), 'f', 1, 32) +
		",latitude=" + strconv.FormatFloat(float64(latitude), 'f', 1, 32) +
		",longtitude=" + strconv.FormatFloat(float64(longitude), 'f', 1, 32) +
		",altitude=" + strconv.FormatFloat(float64(altitude), 'f', 1, 32) +
		" " + strconv.FormatInt(t.UTC().UnixNano(), 10) +
		"\n")

	if err := writeToInfluxDbDustData(influxClient,
		devId,
		*data.ParticularMatter10Um,
		*data.ParticularMatter2_5Um,
		*data.Temperature,
		*data.Humidity,
		latitude,
		longitude,
		float32(altitude),
		t); err != nil {
		fmt.Println(err)
	}
}

func decodeNodeState(appID string,
	devID string,
	req types.UplinkMessage,
	file *os.File,
	influxClient *client.Client) {
	length := binary.Size(req.PayloadRaw)
	fmt.Println("Decoding NodeStatus Protobuf message")
	//Create an struct pointer of type ProtobufTest.TestMessage struct
	protodata := new(protobuf.NodeMessage)
	//Convert all the data retrieved into the ProtobufTest.TestMessage struct type
	err := proto.Unmarshal(req.PayloadRaw[0:length], protodata)
	fmt.Println(protodata.Information)

	if err == nil {
		switch u := protodata.Msg.(type) {
		case *protobuf.NodeMessage_BatteryStateData:
			decodeBatteryState(devID, req.Metadata.Latitude, req.Metadata.Longitude, req.Metadata.Altitude, req.Metadata.Time, u.BatteryStateData, file, influxClient)
		case *protobuf.NodeMessage_DustMeasurementData:
			decodeDustMeasurement(devID, req.Metadata.Latitude, req.Metadata.Longitude, req.Metadata.Altitude, req.Metadata.Time, u.DustMeasurementData, file, influxClient)
		}
		file.Sync()
		fmt.Println(protodata)
	}
}

func test(decodedMessages decoder.ReceivedMessage, doneChannel chan bool) {
	for val := range decodedMessages.BatteryStateData {
		// read from a channel
		fmt.Println(val)
	}
	doneChannel <- true
}
func main() {
	var mqttConfig ttnconnector.MqttClientConfiguration
	mqttConfig.Id = "ttnctl"
	mqttConfig.UserName = "erni-dmn"
	mqttConfig.Password = "ttn-account-v2.qcc_E-maVc_P1uFLeP4KRgBBaE3EF3-Or6u9BU6STac"
	mqttConfig.Brokers = "eu.thethings.network:1883"

	var ttnConnection ttnconnector.TTNConnection
	ttnMessages, error := ttnConnection.Subscribe(mqttConfig, "erni-dmn")
	doneChannel := make(chan bool)
	defer ttnConnection.Unsubscribe()
	if error != nil {
		fmt.Println(error)
	}
	decodedMessages := decoder.Decode(ttnMessages)
	go test(decodedMessages, doneChannel)
	finish := <-doneChannel
	if finish {
		fmt.Println("FINISH ")
	}

	//runtime.Goexit()
}
