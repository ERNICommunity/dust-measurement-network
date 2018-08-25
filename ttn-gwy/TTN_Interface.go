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

func decodeNodeState(appID string, devID string, req types.UplinkMessage) {
	length := binary.Size(req.PayloadRaw)
	fmt.Println("Decoding NodeStatus Protobuf message")
	//Create an struct pointer of type ProtobufTest.TestMessage struct
	protodata := new(protobuf.NodeMessage)
	//Convert all the data retrieved into the ProtobufTest.TestMessage struct type
	err := proto.Unmarshal(req.PayloadRaw[0:length], protodata)
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
			panic(err)
		}
		fmt.Printf("%s", data)
		fmt.Println("")
		fmt.Println("Data Node State: ")
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
		fmt.Println("------Received - UPLINK ------")
		go decodeNodeState(appID, devID, req)
	})
	token.Wait()

	if err := token.Error(); err != nil {
		ctx.WithError(err).Fatal("Could not subscribe")
	}
	runtime.Goexit()

}
