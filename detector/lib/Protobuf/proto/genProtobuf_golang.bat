rmdir %cd%\..\golang
mkdir %cd%\..\golang
protoc --go_out=%cd%\..\golang dustMeasurement.proto
protoc --go_out=%cd%\..\golang applicationInformation.proto
protoc --go_out=%cd%\..\golang batteryState.proto
protoc --go_out=%cd%\..\golang nodeStatus.proto