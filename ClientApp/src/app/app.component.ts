import { Component } from '@angular/core';
import { LoadingProgressService } from './service/loading-progress.service';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'app';
  get isRequestRunning(): boolean {
    return this.requestService.requestCount > 0;
  }
  constructor(private requestService: LoadingProgressService) {
  }
}
