package ttnconnector

import (
	"errors"
	"fmt"
	"time"

	"github.com/TheThingsNetwork/ttn/core/types"
	"github.com/TheThingsNetwork/ttn/mqtt"
)

type Position struct {
	Latitude  float32
	Longitude float32
	Altitude  int32
}
type TTNUplinkMessage struct {
	ApplicationID string
	DeviceID      string
	Position      Position
	PayloadRaw    []byte
	Timestamp     time.Time
	AirTime       time.Duration
}

type MqttClientConfiguration struct {
	Id       string
	UserName string
	Password string
	Brokers  string
}

type ttnConnection struct {
	mqttClient       MqttClientConfiguration
	applicationID    string
	receivedMessages chan TTNUplinkMessage
}

func (c *ttnConnection) handler(client mqtt.Client, appID string, devID string, req types.UplinkMessage) {
	var rx_message TTNUplinkMessage

	rx_message.ApplicationID = appID
	rx_message.DeviceID = devID
	rx_message.PayloadRaw = req.PayloadRaw
	rx_message.Position.Altitude = req.Metadata.Altitude
	rx_message.Position.Latitude = req.Metadata.Latitude
	rx_message.Position.Longitude = req.Metadata.Longitude
	//rx_message.Timestamp = (time.Time)req.Metadata.Time
	rx_message.AirTime = req.Metadata.Airtime

	c.receivedMessages <- rx_message
}

func (c *ttnConnection) uplinkHandler() error {
	fmt.Println("TRY TO CONNECT " + c.mqttClient.UserName)
	mqttClient := mqtt.NewClient(nil, c.mqttClient.Id, c.mqttClient.UserName, c.mqttClient.Password, c.mqttClient.Brokers)
	err := mqttClient.Connect()
	if err != nil {

		return errors.New("MQTT-Client: Could not connect")

	}
	token := mqttClient.SubscribeAppUplink(c.applicationID, c.handler)
	token.Wait()
	err = token.Error()
	if err != nil {
		err = errors.New("MQTT-Client: Could not subscribe")
	}
	return err
}

func Subscribe(mqttClientConfig MqttClientConfiguration, applicationID string) (<-chan TTNUplinkMessage, error) {
	var connection ttnConnection
	connection.mqttClient = mqttClientConfig
	connection.applicationID = applicationID
	connection.receivedMessages = make(chan TTNUplinkMessage)
	err := connection.uplinkHandler()
	return connection.receivedMessages, err
}
