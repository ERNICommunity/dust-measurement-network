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
  private requestWatcherSubscription: Subscription;

  constructor(
    private requestService: RunningRequestService,
    private loadingBar: LoadingBarService) {}

  ngOnInit(): void {
    this.requestWatcherSubscription = this.requestService.watcher.subscribe(x => x ? this.loadingBar.start() : this.loadingBar.complete() );
  }
  ngOnDestroy(): void {
    this.requestWatcherSubscription.unsubscribe();
  }
}
