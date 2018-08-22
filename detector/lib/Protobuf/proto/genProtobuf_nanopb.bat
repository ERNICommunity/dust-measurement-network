protoc --plugin=protoc-gen-nanopb=%cd%\..\generator\protoc-gen-nanopb.bat --nanopb_out=%cd%\..\ dustMeasurement.proto
protoc --plugin=protoc-gen-nanopb=%cd%\..\generator\protoc-gen-nanopb.bat --nanopb_out=%cd%\..\ applicationInformation.proto
protoc --plugin=protoc-gen-nanopb=%cd%\..\generator\protoc-gen-nanopb.bat --nanopb_out=%cd%\..\ batteryState.proto
protoc --plugin=protoc-gen-nanopb=%cd%\..\generator\protoc-gen-nanopb.bat --nanopb_out=%cd%\..\ nodeStatus.proto  