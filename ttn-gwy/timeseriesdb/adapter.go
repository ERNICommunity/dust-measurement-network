package timeseriesdb

import (
	"fmt"

	"../decoder"
)

func writeDustMeasurementData(chanDustMeasurementState <-chan decoder.ReceivedDustMeasurement) {
	for val := range chanDustMeasurementState {
		// read from a channel
		fmt.Println(val)
	}
}

func writeBatteryState(chanBatteryState <-chan decoder.ReceivedBatteryStateData) {
	for val := range chanBatteryState {
		// read from a channel
		fmt.Println(val)
	}
}

func WriteData(receivedMassages decoder.ReceivedMessage) {
	go writeBatteryState(receivedMassages.BatteryStateData)
	go writeDustMeasurementData(receivedMassages.DustMeasurementData)
}
