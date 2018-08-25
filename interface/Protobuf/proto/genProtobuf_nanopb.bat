SET OUTPUT_DIRECTORY=%cd%\..\..\..\detector\lib\Protobuf\
SET NANOPB_GENERATOR=protoc-gen-nanopb=%cd%\..\generator\protoc-gen-nanopb.bat
protoc --plugin=%NANOPB_GENERATOR% --nanopb_out=%OUTPUT_DIRECTORY% dustMeasurement.proto
protoc --plugin=%NANOPB_GENERATOR% --nanopb_out=%OUTPUT_DIRECTORY% applicationInformation.proto
protoc --plugin=%NANOPB_GENERATOR% --nanopb_out=%OUTPUT_DIRECTORY% batteryState.proto
protoc --plugin=%NANOPB_GENERATOR% --nanopb_out=%OUTPUT_DIRECTORY% nodeStatus.proto  