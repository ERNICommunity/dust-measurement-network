import { Component, OnInit, OnDestroy } from '@angular/core';
import { LoadingProgressService } from './service/loading-progress.service';
import { Subscription } from 'rxjs';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit, OnDestroy {
  private requestWatcherSubscription: Subscription;

  constructor(private requestService: LoadingProgressService) {}

  ngOnInit(): void {
    this.requestWatcherSubscription = this.requestService.watcher.subscribe(x => console.log('some requests running;', x));
  }
  ngOnDestroy(): void {
    this.requestWatcherSubscription.unsubscribe();
  }
}
