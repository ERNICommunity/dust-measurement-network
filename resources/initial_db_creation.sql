-- ****************** SqlDBM: Microsoft SQL Server ******************
-- ******************************************************************

DROP TABLE [SensorData];
GO
  

DROP TABLE [SensorDataEnhancement];
GO


DROP TABLE [Sensor];
GO

--************************************** [SensorDataEnhancement]

CREATE TABLE [SensorDataEnhancement]
(
 [EnhancementId] BIGINT NOT NULL ,
 [windspeed]     FLOAT NOT NULL ,
 [winddirection] SMALLINT NOT NULL ,
 [rainamount]    SMALLINT NOT NULL ,

 CONSTRAINT [PK_SensorDataEnhancement] PRIMARY KEY CLUSTERED ([EnhancementId] ASC)
);
GO



--************************************** [Sensor]

CREATE TABLE [Sensor]
(
 [SensorId]  INT NOT NULL ,
 [longitude] FLOAT NOT NULL ,
 [latitude]  FLOAT NOT NULL ,
 [altitude]  FLOAT NOT NULL ,

 CONSTRAINT [PK_Sensor] PRIMARY KEY CLUSTERED ([SensorId] ASC)
);
GO



--************************************** [SensorData]

CREATE TABLE [SensorData]
(
 [DataId]               BIGINT NOT NULL ,
 [SensorId]             INT NOT NULL ,
 [EnhancementId]        BIGINT NOT NULL ,
 [timestamp]            DATETIME NOT NULL ,
 [particulatematter25]  FLOAT NOT NULL ,
 [particulatematter100] FLOAT NOT NULL ,
 [airtemperature]       FLOAT NOT NULL ,
 [relativehumidity]     FLOAT NOT NULL ,

 CONSTRAINT [PK_InfluxData] PRIMARY KEY CLUSTERED ([DataId] ASC, [SensorId] ASC, [EnhancementId] ASC),
 CONSTRAINT [FK_39] FOREIGN KEY ([SensorId])
  REFERENCES [Sensor]([SensorId]),
 CONSTRAINT [FK_43] FOREIGN KEY ([EnhancementId])
  REFERENCES [SensorDataEnhancement]([EnhancementId])
);
GO


--SKIP Index: [fkIdx_39]

--SKIP Index: [fkIdx_43]


