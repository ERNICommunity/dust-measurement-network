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

type TTNConnection struct {
	mqttClient       MqttClientConfiguration
	client           mqtt.Client
	applicationID    string
	receivedMessages chan TTNUplinkMessage
}

func (c *TTNConnection) handler(client mqtt.Client, appID string, devID string, req types.UplinkMessage) {
	var rx_message TTNUplinkMessage
	fmt.Println("--- RECEIVED MESSAGE ---")
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

func (c *TTNConnection) uplinkHandler() error {
	fmt.Println("TRY TO CONNECT " + c.mqttClient.UserName)
	c.client = mqtt.NewClient(nil, c.mqttClient.Id, c.mqttClient.UserName, c.mqttClient.Password, c.mqttClient.Brokers)
	err := c.client.Connect()
	if err != nil {

		return errors.New("MQTT-Client: Could not connect")

	}
	token := c.client.SubscribeAppUplink(c.applicationID, c.handler)
	token.Wait()
	err = token.Error()
	if err != nil {
		err = errors.New("MQTT-Client: Could not subscribe")
	}
	return err
}
func (r *TTNConnection) Unsubscribe() {
	token := r.client.UnsubscribeAppUplink(r.applicationID)
	token.Wait()
	fmt.Println("TTN DISCONNECT")
	r.client.Disconnect()
}
func (r *TTNConnection) Subscribe(mqttClientConfig MqttClientConfiguration, applicationID string) (<-chan TTNUplinkMessage, error) {
	r.mqttClient = mqttClientConfig
	r.applicationID = applicationID
	r.receivedMessages = make(chan TTNUplinkMessage)
	err := r.uplinkHandler()
	return r.receivedMessages, err
}
