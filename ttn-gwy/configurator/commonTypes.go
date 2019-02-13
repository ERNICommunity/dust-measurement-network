package configurator

// Client clients to show the results
type Client struct {
	ID    int    `json:"id"`
	Place string `json:"place"`
	URL   string `json:"url"`
}

// Client clients to show the results
type HostMqtt struct {
	ID       string   `json:"id"`
	Username string   `json:"username"`
	Password string   `json:"password"`
	Brokers  []string `json:"brokers"`
}

type TimeSeriesDataBase struct {
	Address  string `json:"address"`
	Port     int    `json:"port"`
	Username string `json:"username"`
	Password string `json:"password"`
	Enable   bool   `json:"true"`
}

type TimeSeriesLog struct {
	Path        string `json:"path"`
	Enable      bool   `json:"enable"`
	MaxSizeByte int    `json:"maxSizeByte"`
}

type TimeSeries struct {
	DataBase TimeSeriesDataBase `json:"database"`
	LogFile  TimeSeriesLog      `json:"logFile"`
}

type Hosts struct {
	Mqtt       HostMqtt   `json:"mqtt"`
	TimeSeries TimeSeries `json:"timeSeries"`
}

type Root struct {
	HostsSettings Hosts `json:"hosts"`
}
