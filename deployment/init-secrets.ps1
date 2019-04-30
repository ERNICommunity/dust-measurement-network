
Add-Type -AssemblyName System.web

function GeneratePassword() {
    [System.Web.Security.Membership]::GeneratePassword(32,4)
}

# Postgres
kubectl create secret generic postgresql-user-pass --from-literal=username=$(GeneratePassword) --from-literal=password=$(GeneratePassword) --dry-run -o yaml > .\secrets\postgresql.yaml

# Influx DB
kubectl create secret generic influxdb-user-pass --from-literal=username=$(GeneratePassword) --from-literal=password=$(GeneratePassword) --dry-run -o yaml > .\secrets\influxdb.yaml