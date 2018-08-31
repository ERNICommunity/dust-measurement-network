import { Injectable, Inject } from '@angular/core';
import { HttpClient, HttpErrorResponse } from '@angular/common/http';
import { SensorData } from './definitions/SensorData';
import { DustData } from './definitions/DustData';

@Injectable()
export class DustService {

  constructor(private httpClient: HttpClient, @Inject('BASE_URL') private baseUrl: string) { }

  public getSensorData(): Promise<SensorData[]> {
    const url = '/api/sensors';
    return this.httpClient.get<SensorData[]>(url).toPromise()
        .catch((response: HttpErrorResponse) => {
            throw new Error('Unexpected error (HTTP status ' + response.status + ')');
        });
  }

  public getDustHistory(id: number, from: Date, to: Date) {
    return this.httpClient.get<DustData[]>(`${this.baseUrl}api/sensors/${id}/history?from=${from.getTime()}&to=${to.getTime()}`);
  }

  public getDustPrediction(id: number) {
    return this.httpClient.get<DustData[]>(`${this.baseUrl}api/sensors/${id}/prediction`);
  }

}
