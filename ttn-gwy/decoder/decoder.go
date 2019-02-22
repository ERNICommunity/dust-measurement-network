package decoder

import (
	"encoding/binary"

	"fmt"

	"../protobuf"
	"../ttnconnector"
	"github.com/gogo/protobuf/proto"
)

type ReceivedMessage struct {
	DustMeasurementData <-chan ReceivedDustMeasurement
	BatteryStateData    <-chan ReceivedBatteryStateData
}

type channelsToDecode struct {
	dustMeasurementData chan ReceivedDustMeasurement
	batteryStateData    chan ReceivedBatteryStateData
}

func decodeBatteryState(data *protobuf.BatteryState) BatteryStateData {
	var decodedState BatteryStateData
	decodedState.Voltage = *data.Voltage
	decodedState.Health = float32(*data.State)
	return decodedState
}

func decodeDustMeasurementData(data *protobuf.DustSensorMeasurement) DustMeasurementData {
	var decodedDustMeasurementData DustMeasurementData
	decodedDustMeasurementData.ParticularMatter10uM = *data.ParticularMatter10Um
	decodedDustMeasurementData.ParticularMatter25uM = *data.ParticularMatter2_5Um
	decodedDustMeasurementData.HumidityPercent = *data.Humidity
	decodedDustMeasurementData.TemperatureC = *data.Temperature
	return decodedDustMeasurementData
}

func (r *channelsToDecode) decodeMessages(receivedTTNDataChannel <-chan ttnconnector.TTNUplinkMessage) {
	for incommingMessage := range receivedTTNDataChannel {
		fmt.Println("----- WAIT FOR DATA ------")
		var head ReceivedMessageHead
		fmt.Println(incommingMessage.ApplicationID)
		head.ApplicationID = incommingMessage.ApplicationID
		head.DeviceID = incommingMessage.DeviceID
		head.Position.Altitude = float32(incommingMessage.Position.Altitude)
		head.Position.Latitude = incommingMessage.Position.Latitude
		head.Position.Longitude = incommingMessage.Position.Longitude
		head.Timestamp = incommingMessage.Timestamp
		head.AirTime = incommingMessage.AirTime

		length := binary.Size(incommingMessage.PayloadRaw)
		//Create an struct pointer of type ProtobufTest.TestMessage struct
		protodata := new(protobuf.NodeMessage)
		//Convert all the data retrieved into the ProtobufTest.TestMessage struct type
		err := proto.Unmarshal(incommingMessage.PayloadRaw[0:length], protodata)

		if err == nil {
			switch u := protodata.Msg.(type) {
			case *protobuf.NodeMessage_BatteryStateData:
				var rxBatteryData ReceivedBatteryStateData
				rxBatteryData.Head = head
				rxBatteryData.Data = decodeBatteryState(u.BatteryStateData)
				r.batteryStateData <- rxBatteryData
			case *protobuf.NodeMessage_DustMeasurementData:
				var rxDustMeasearementData ReceivedDustMeasurement
				rxDustMeasearementData.Head = head
				rxDustMeasearementData.Data = decodeDustMeasurementData(u.DustMeasurementData)
				r.dustMeasurementData <- rxDustMeasearementData
			}
		}
	}
}
func Decode(receivedTTNDataChannel <-chan ttnconnector.TTNUplinkMessage) ReceivedMessage {
	var decodedMessages channelsToDecode
	decodedMessages.dustMeasurementData = make(chan ReceivedDustMeasurement)
	decodedMessages.batteryStateData = make(chan ReceivedBatteryStateData)
	go decodedMessages.decodeMessages(receivedTTNDataChannel)
	var retDecodedMessages ReceivedMessage
	retDecodedMessages.BatteryStateData = decodedMessages.batteryStateData
	retDecodedMessages.DustMeasurementData = decodedMessages.dustMeasurementData
	return retDecodedMessages
}
