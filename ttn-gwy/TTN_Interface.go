package main

import (
	"encoding/binary"
	"fmt"
	"os"
	"runtime"
	"strconv"
	"time"

	"./protobuf"
	"github.com/TheThingsNetwork/go-utils/log"
	"github.com/TheThingsNetwork/go-utils/log/apex"
	"github.com/TheThingsNetwork/ttn/core/types"
	"github.com/TheThingsNetwork/ttn/mqtt"
	"github.com/gogo/protobuf/proto"
)

/*var {
	fileData *os.File
}*/

type MqttConfiguration struct {
	ApplicationId  string
	ApplicationKey string
	broker         string
}

func checkError(err error) {
	if err != nil {
		fmt.Fprintf(os.Stderr, "Fatal error: %s", err.Error())
		//os.Exit(1)
	}
}

func decodeBatteryState(devId string,
	latitude float32,
	longitude float32,
	altitude int32,
	timestamp types.JSONTime,
	data *protobuf.BatteryState, file *os.File) {
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
}

func decodeDustMeasurement(devId string,
	latitude float32,
	longitude float32,
	altitude int32,
	timestamp types.JSONTime,
	data *protobuf.DustSensorMeasurement, file *os.File) {
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
}

func decodeNodeState(appID string, devID string, req types.UplinkMessage, file *os.File) {
	length := binary.Size(req.PayloadRaw)
	fmt.Println("Decoding NodeStatus Protobuf message")
	//Create an struct pointer of type ProtobufTest.TestMessage struct
	protodata := new(protobuf.NodeMessage)
	//Convert all the data retrieved into the ProtobufTest.TestMessage struct type
	err := proto.Unmarshal(req.PayloadRaw[0:length], protodata)
	fmt.Println(protodata.Information)

	if err == nil {
		fmt.Println("DEVICE ID ")
		fmt.Println(devID)
		fmt.Println("META DATA PLACE: ")
		fmt.Println(req.Metadata.Latitude)
		fmt.Println(req.Metadata.Longitude)
		fmt.Println(req.Metadata.Altitude)
		fmt.Print("META DATA TIME: ")
		data, err := req.Metadata.Time.MarshalText()
		if err != nil {
			//panic(err)
			fmt.Println("error")
		}
		fmt.Printf("%s", data)
		fmt.Println("")
		fmt.Println("Data Node State: ")
		fmt.Println()
		/*amountOfString, err := f.WriteString("writes\n")

		f.Sync()*/
		switch u := protodata.Msg.(type) {
		case *protobuf.NodeMessage_BatteryStateData:
			decodeBatteryState(devID, req.Metadata.Latitude, req.Metadata.Longitude, req.Metadata.Altitude, req.Metadata.Time, u.BatteryStateData, file)
		case *protobuf.NodeMessage_DustMeasurementData:
			decodeDustMeasurement(devID, req.Metadata.Latitude, req.Metadata.Longitude, req.Metadata.Altitude, req.Metadata.Time, u.DustMeasurementData, file)
		}
		file.Sync()
		fmt.Println(protodata)
	}
}

func main() {
	ctx := apex.Stdout().WithField("Example", "Go Client")
	log.Set(ctx)

	client := mqtt.NewClient(ctx, "ttnctl", "erni-hello-world", "ttn-account-v2.bWBs5y8lsFgMuZC9zG01vd_faBksM0acxkxrpzm7zwg", "eu.thethings.network:1883")
	if err := client.Connect(); err != nil {
		ctx.WithError(err).Fatal("Could not connect")
	}

	file, err := os.Create("output.txt")
	checkError(err)
	//defer file.Close()

	token := client.SubscribeAppUplink("erni-hello-world", func(client mqtt.Client, appID string, devID string, req types.UplinkMessage) {
		fmt.Println("------Received - UPLINK ------")
		go decodeNodeState(appID, devID, req, file)
	})
	token.Wait()

	if err := token.Error(); err != nil {
		ctx.WithError(err).Fatal("Could not subscribe")
	}
	runtime.Goexit()

}
