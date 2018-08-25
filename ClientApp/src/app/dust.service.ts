import { Injectable, Inject } from '@angular/core';
import { HttpClient, HttpErrorResponse, HttpHeaders, HttpParams } from "@angular/common/http";
import { SensorData } from "./definitions/SensorData";
import { DustData } from "./definitions/DustData";

@Injectable()
export class DustService {

  constructor(private httpClient: HttpClient, @Inject('BASE_URL') private baseUrl: string) { }

  public getSensorData(): Promise<SensorData[]> {
    let url = "/api/sensors";
    return this.httpClient.get<SensorData[]>(url).toPromise()
        .catch((response: HttpErrorResponse) => {
            throw 'Unexpected error (HTTP status ' + response.status + ')';
        });
  }

  /*public getDustHistory(id: number): Promise<DustData[]> {
    let url = "/api/sensors/" + id + "/history";
    return this.httpClient.get<DustData[]>(url).toPromise()
        .catch((response: HttpErrorResponse) => {
            throw 'Unexpected error (HTTP status ' + response.status + ')';
        });
  }*/

  public getDustPrediction(id: number): Promise<DustData[]> {
    let url = "/api/sensors/" + id + "/prediction";
    return this.httpClient.get<DustData[]>(url).toPromise()
        .catch((response: HttpErrorResponse) => {
            throw 'Unexpected error (HTTP status ' + response.status + ')';
        });
  }

  public getDustHistory(id: number, callback: Function) {
    this.httpClient.get<DustData[]>(this.baseUrl + "api/sensors/" + id + "/history").subscribe(
      result => callback(result),
      error => console.error(error));
  }

}
