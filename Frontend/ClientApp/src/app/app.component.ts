import { Component, OnInit, OnDestroy } from '@angular/core';
import { RunningRequestService } from './service/running-request.service';
import { Subscription } from 'rxjs';
import { LoadingBarService } from '@ngx-loading-bar/core';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit, OnDestroy {
  private _requestWatcherSubscription: Subscription;

  constructor(
    private _requestService: RunningRequestService,
    private _loadingBar: LoadingBarService) {}

  ngOnInit(): void {
    this._requestWatcherSubscription = this._requestService.watcher.subscribe(x => x
      ? this._loadingBar.start()
      : this._loadingBar.complete() );
  }
  ngOnDestroy(): void {
    this._requestWatcherSubscription.unsubscribe();
  }
}
