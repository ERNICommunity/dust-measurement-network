#!/bin/bash
if [ "$(docker network ls | grep influxdb)" ]; then
      docker network rm influxdb
fi

if [ ! "$(docker ps -q -f name=influxdb)" ]; then

if [ ! "$(docker ps -q -f name=influxdb -f status=running)" ]; then
      # stop
      docker stop influxdb
fi


      # stop
      docker rm influxdb
fi

if [ ! "$(docker ps -q -f name=chronograf)" ]; then

if [ ! "$(docker ps -q -f name=chronograf -f status=running)" ]; then
      # stop
      docker stop chronograf
fi
      # stop
      docker rm chronograf
fi

docker network create influxdb &&

docker run -p 8086:8086 \
      -v $PWD/influxdb.conf:/etc/influxdb/influxdb.conf:ro \
      -e INFLUXDB_DB=db0 -e INFLUXDB_ADMIN_ENABLED=true \
      -e INFLUXDB_ADMIN_USER=admin -e INFLUXDB_ADMIN_PASSWORD=supersecretpassword \
      -e INFLUXDB_USER=telegraf -e INFLUXDB_USER_PASSWORD=secretpassword \
      -d --name=influxdb \
      --net=influxdb \
      influxdb -config /etc/influxdb/influxdb.conf /init-influxdb.sh &

docker run -p 8888:8888 \
      -d --name=chronograf \
      --net=influxdb \
      chronograf --influxdb-url=http://influxdb:8086 &
