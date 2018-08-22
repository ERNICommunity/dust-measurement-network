package main

import (
	"encoding/binary"
	"fmt"
	"os"
	"runtime"

	"./protobuf"
	"github.com/TheThingsNetwork/go-utils/log"
	"github.com/TheThingsNetwork/go-utils/log/apex"
	"github.com/TheThingsNetwork/ttn/core/types"
	"github.com/TheThingsNetwork/ttn/mqtt"
	"github.com/gogo/protobuf/proto"
)

func checkError(err error) {
	if err != nil {
		fmt.Fprintf(os.Stderr, "Fatal error: %s", err.Error())
		//os.Exit(1)
	}
}

func decodeDustMeasurement(req types.UplinkMessage) {
	length := binary.Size(req.PayloadRaw)
	fmt.Println("Decoding DustMeasurement Protobuf message")
	//Create an struct pointer of type ProtobufTest.TestMessage struct
	protodata := new(protobuf.DustSensorMeasurement)
	//Convert all the data retrieved into the ProtobufTest.TestMessage struct type
	err := proto.Unmarshal(req.PayloadRaw[0:length], protodata)
	if err == nil {
		fmt.Print("Data Dust Measurement: ")
		fmt.Println(protodata)
	}
}

func decodeBatteryState(req types.UplinkMessage) {
	length := binary.Size(req.PayloadRaw)
	fmt.Println("Decoding BatteryState Protobuf message")
	//Create an struct pointer of type ProtobufTest.TestMessage struct
	protodata := new(protobuf.BatteryState)
	//Convert all the data retrieved into the ProtobufTest.TestMessage struct type
	err := proto.Unmarshal(req.PayloadRaw[0:length], protodata)
	if err == nil {
		fmt.Print("Data Battery State: ")
		fmt.Println(protodata)
	}
}

func decodeNodeState(req types.UplinkMessage) {
	length := binary.Size(req.PayloadRaw)
	fmt.Println("Decoding NodeStatus Protobuf message")
	//Create an struct pointer of type ProtobufTest.TestMessage struct
	protodata := new(protobuf.NodeMessage)
	//Convert all the data retrieved into the ProtobufTest.TestMessage struct type
	err := proto.Unmarshal(req.PayloadRaw[0:length], protodata)
	if err == nil {
		fmt.Print("Data Node State: ")
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
	fmt.Println("HELLO WORLD")

	token := client.SubscribeAppUplink("erni-hello-world", func(client mqtt.Client, appID string, devID string, req types.UplinkMessage) {
		fmt.Println("Received - UPLINK")
		fmt.Println(devID)
		go decodeNodeState(req)
	})
	token.Wait()

	if err := token.Error(); err != nil {
		ctx.WithError(err).Fatal("Could not subscribe")
	}
	runtime.Goexit()

}
