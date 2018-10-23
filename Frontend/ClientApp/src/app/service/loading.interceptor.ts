import { Injectable } from '@angular/core';
import { RunningRequestService } from './running-request.service';
import { HttpInterceptor, HttpRequest, HttpHandler } from '@angular/common/http';
import { Observable } from 'rxjs';
import { finalize } from 'rxjs/operators';

@Injectable()
export class LoadingInterceptor implements HttpInterceptor {

  constructor(private requestService: RunningRequestService) { }

  intercept(req: HttpRequest<any>, next: HttpHandler): Observable<any> {
    this.requestService.requestStarted();
    return next.handle(req).pipe(finalize(() => this.requestService.requestFinished()));
  }
}
