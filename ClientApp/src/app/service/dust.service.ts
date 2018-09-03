import { Injectable, Inject } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { SensorDto } from './SensorDto';
import { DustDto } from './DustDto';

import { concat, finalize } from 'rxjs/operators';
import { Observable } from 'rxjs';
import { empty } from 'rxjs';
import { RequestService } from './request.service';

@Injectable()
export class DustService {
  private reqCount = 0;

  constructor(
    private httpClient: HttpClient,
    @Inject('BASE_URL') private baseUrl: string,
    private requestService: RequestService
  ) { }

  public getSensors(minLon: number, minLat: number , maxLon: number, maxLat: number) {
    return this.wrap(this.httpClient.get<SensorDto[]>(`${this.baseUrl}api/sensors?minLon=${minLon}&minLat=${minLat}&maxLon=${maxLon}&maxLat=${maxLat}`));
  }

  public getDustHistory(id: number, from: Date, to: Date) {
    return this.wrap(this.httpClient.get<DustDto[]>(`${this.baseUrl}api/sensors/${id}/history?from=${from.getTime()}&to=${to.getTime()}`));
  }

  public getDustPrediction(id: number) {
    return this.wrap(this.httpClient.get<DustDto[]>(`${this.baseUrl}api/sensors/${id}/prediction`));
  }

  private wrap<T>(o: Observable<T>) {
    return empty().pipe(finalize(() => this.requestService.requestCount++)).pipe(concat(o)).pipe(finalize(() => this.requestService.requestCount--));
  }
}
