import { Injectable, Inject } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { SensorDto } from './SensorDto';
import { DustDto } from './DustDto';

@Injectable()
export class DustService {

  constructor(
    private _httpClient: HttpClient,
    @Inject('BASE_URL') private _baseUrl: string
  ) { }

  public getSensors(minLon: number, minLat: number , maxLon: number, maxLat: number) {
    return this._httpClient.get<SensorDto[]>(`${this._baseUrl}api/sensors?minLon=${minLon}&minLat=${minLat}&maxLon=${maxLon}&maxLat=${maxLat}`);
  }

  public getDustHistory(id: number, from: Date, to: Date) {
    return this._httpClient.get<DustDto[]>(`${this._baseUrl}api/sensors/${id}/history?from=${from.getTime()}&to=${to.getTime()}`);
  }

  public getDustPrediction(id: number) {
    return this._httpClient.get<DustDto[]>(`${this._baseUrl}api/sensors/${id}/prediction`);
  }
}
