import { Injectable, Inject } from '@angular/core';
import { HttpClient, HttpErrorResponse } from '@angular/common/http';
import { SensorDto } from './SensorDto';
import { DustDto } from './DustDto';

@Injectable()
export class DustService {

  constructor(private httpClient: HttpClient, @Inject('BASE_URL') private baseUrl: string) { }

  public getSensors(minLon: number, minLat: number , maxLon: number, maxLat: number) {
    return this.httpClient.get<SensorDto[]>(`${this.baseUrl}api/sensors?minLon=${minLon}&minLat=${minLat}&maxLon=${maxLon}&maxLat=${maxLat}`);
  }

  public getDustHistory(id: number, from: Date, to: Date) {
    return this.httpClient.get<DustDto[]>(`${this.baseUrl}api/sensors/${id}/history?from=${from.getTime()}&to=${to.getTime()}`);
  }

  public getDustPrediction(id: number) {
    return this.httpClient.get<DustDto[]>(`${this.baseUrl}api/sensors/${id}/prediction`);
  }

}
