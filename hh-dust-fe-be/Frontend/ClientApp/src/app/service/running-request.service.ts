import { Injectable } from '@angular/core';
import { Subject } from 'rxjs';

@Injectable()
export class RunningRequestService {
  private _runningRequestsCount = 0;
  private _subject = new Subject<boolean>();

  watcher = this._subject.asObservable();

  constructor() { }

  requestStarted() {
    this._runningRequestsCount++;
    if (this._runningRequestsCount === 1) {
      this._subject.next(true);
    }
  }

  requestFinished() {
    this._runningRequestsCount--;
    if (this._runningRequestsCount === 0) {
      this._subject.next(false);
    }
  }
}
