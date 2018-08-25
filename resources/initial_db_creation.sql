-- Postgres Script

ALTER TABLE "SensorData" DROP CONSTRAINT IF EXISTS "SensorData_fk0";
ALTER TABLE "SensorData" DROP CONSTRAINT IF EXISTS "SensorData_fk1";

DROP TABLE IF EXISTS "Sensor";
DROP TABLE IF EXISTS "SensorData";
DROP TABLE IF EXISTS "SensorDataEnhancement";

CREATE TABLE "Sensor" (
	"sensorid" serial NOT NULL,
	"longitude" FLOAT NOT NULL,
	"latitude" FLOAT NOT NULL,
	"altitude" FLOAT NOT NULL,
	CONSTRAINT Sensor_pk PRIMARY KEY ("sensorid")
) WITH (
  OIDS=FALSE
);

CREATE TABLE "SensorData" (
	"dataid" serial NOT NULL,
	"timestamp" TIMESTAMP NOT NULL,
	"particulatematter25" FLOAT NOT NULL,
	"particulatematter100" FLOAT NOT NULL,
	"airtemperature" FLOAT NOT NULL,
	"relativehumidity" FLOAT NOT NULL,	
	"sensorid" integer NOT NULL,
	"enhancementid" integer,
	CONSTRAINT SensorData_pk PRIMARY KEY ("dataid")
) WITH (
  OIDS=FALSE
);

CREATE TABLE "SensorDataEnhancement" (
	"enhancementid" serial NOT NULL,
	"winddirection" FLOAT,
	"windspeed" FLOAT NOT NULL,
	"rainamount" FLOAT NOT NULL,
	CONSTRAINT SensorDataEnhancement_pk PRIMARY KEY ("enhancementid")
) WITH (
  OIDS=FALSE
);

ALTER TABLE "SensorData" ADD CONSTRAINT "SensorData_fk0" FOREIGN KEY ("sensorid") REFERENCES "Sensor"("sensorid");
ALTER TABLE "SensorData" ADD CONSTRAINT "SensorData_fk1" FOREIGN KEY ("enhancementid") REFERENCES "SensorDataEnhancement"("enhancementid");