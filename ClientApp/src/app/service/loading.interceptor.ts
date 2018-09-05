import { Injectable } from '@angular/core';
import { LoadingProgressService } from './loading-progress.service';
import { HttpInterceptor, HttpRequest, HttpHandler } from '@angular/common/http';
import { Observable } from 'rxjs';
import { finalize } from 'rxjs/operators';

@Injectable()
export class LoadingInterceptor implements HttpInterceptor {

  constructor(private loadingService: LoadingProgressService) { }

  intercept(req: HttpRequest<any>, next: HttpHandler): Observable<any> {
    this.loadingService.requestStarted();

    return next.handle(req).pipe(finalize(() => this.loadingService.requestFinished()));
  }
}
