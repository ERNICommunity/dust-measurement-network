#!/bin/bash
docker run -p 8888:8888 \
      --net=influxdb \
      chronograf --influxdb-url=http://influxdb:8086