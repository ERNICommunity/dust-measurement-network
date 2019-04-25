FROM golang:1.10

WORKDIR /go/src/app
COPY . .

RUN go get -d -v github.com/TheThingsNetwork/go-utils/log 
RUN go get -d -v github.com/TheThingsNetwork/ttn/core/types
RUN go get -d -v github.com/TheThingsNetwork/ttn/mqtt
RUN go get -d -v github.com/apex/log
RUN go get -d -v github.com/gogo/protobuf/proto
RUN go get -d -v github.com/golang/protobuf/proto
RUN go get -d -v github.com/influxdata/influxdb1-client/v2
RUN go get -d -v github.com/influxdata/platform

RUN go build -o ttn_gateway_app main.go
CMD ["./ttn_gateway_app"]