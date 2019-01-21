# Deployment

DMN is deployed to kubernetes cluster in Azure.

Following components are currently running:
* PostgreSQL [51.144.114.5](51.144.114.5)
* InfluxDB [104.40.196.109](104.40.196.109)
* Chronograf [http://137.117.132.251](http://137.117.132.251)

Contact person: (mailto:michal.dorner@erni.sk)[Michal Dorner]

Future work:
* migrate to ERNI Azure account and dedicated resource group.
* manage credentials
* deploy backend & frontend containers


## Kubernetes cheat sheet

View kubernetes dashboard
```
az aks browse --resource-group DMN --name dmn
```

Apply configuration
```
kubectl apply -f <PATH> -R
```

List resources
```
kubectl get nodes
kubectl get pods
kubectl get statefulsets
kubectl get services
kubectl get pvc
```

Delete resource
```
kubectl delete <KIND> <NAME>
```

Show infromation about resoruce
```
kubectl desribe <KIND> <NAME>
```

Show logs
```
kubectl logs <POD> <CONTAINER?>
```

Connect to InfluxDB shell
```
kubectl exec -i -t influxdb-0 -- influx -username <USER> -password <PASSWORD>
```

