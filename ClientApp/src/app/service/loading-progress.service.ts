import { Injectable } from '@angular/core';

@Injectable()
export class LoadingProgressService {
  requestCount = 0;
  constructor() { }
}
