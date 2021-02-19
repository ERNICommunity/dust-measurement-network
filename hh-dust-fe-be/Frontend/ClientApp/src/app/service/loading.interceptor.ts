import { Injectable } from '@angular/core';
import { RunningRequestService } from './running-request.service';
import { HttpInterceptor, HttpRequest, HttpHandler, HttpEvent } from '@angular/common/http';
import { Observable } from 'rxjs';
import { finalize } from 'rxjs/operators';

@Injectable()
export class LoadingInterceptor implements HttpInterceptor {

  constructor(private _requestService: RunningRequestService) { }

  intercept(req: HttpRequest<any>, next: HttpHandler): Observable<HttpEvent<any>> {
    this._requestService.requestStarted();
    return next.handle(req).pipe(finalize(() => this._requestService.requestFinished()));
  }
}
