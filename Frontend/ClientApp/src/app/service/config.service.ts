import { Injectable, Injector } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { ConfigDto } from './config.dto';

@Injectable()
export class ConfigService {
  private static _autorefreshInterval: number;

  get autorefreshInterval(): number {
    return ConfigService._autorefreshInterval;
  }

  private static initialize(httpClient: HttpClient, baseUrl: string)  {
    return httpClient.get<ConfigDto>(`${baseUrl}api/clientConfiguration`)
      .toPromise()
      .then(c => this._autorefreshInterval = c.autorefreshIntervalMilisec);
  }

  static factory(httpClient: HttpClient, injector: Injector) {
    return () => ConfigService.initialize(httpClient, injector.get('BASE_URL'));
  }
}
