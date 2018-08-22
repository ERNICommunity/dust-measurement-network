SET OUTPUT_DIRECTORY=%cd%\..\..\..\ttn-gwy\protobuf

mkdir %OUTPUT_DIRECTORY%

protoc --go_out=%OUTPUT_DIRECTORY% dustMeasurement.proto
protoc --go_out=%OUTPUT_DIRECTORY% applicationInformation.proto
protoc --go_out=%OUTPUT_DIRECTORY% batteryState.proto
protoc --go_out=%OUTPUT_DIRECTORY% nodeStatus.proto