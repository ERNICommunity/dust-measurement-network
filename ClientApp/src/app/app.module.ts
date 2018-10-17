import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpClientModule, HTTP_INTERCEPTORS } from '@angular/common/http';
import { RouterModule } from '@angular/router';

import { LoadingBarModule } from '@ngx-loading-bar/core';
import { NgbModule } from '@ng-bootstrap/ng-bootstrap';

import { AppComponent } from './app.component';
import { NavMenuComponent } from './nav-menu/nav-menu.component';
import { AboutComponent } from './about/about.component';
import { OsmMapComponent } from './osmmap/osmmap.component';
import { HistoryChartComponent } from './history-chart/history-chart.component';
import { DustService } from './service/dust.service';
import { LoadingProgressService } from './service/loading-progress.service';
import { LoadingInterceptor } from './service/loading.interceptor';
import { PopupComponent } from './popup/popup.component';
import { PredictionChartComponent } from './prediction-chart/prediction-chart.component';
import { LegendComponent } from './legend/legend.component';

@NgModule({
  declarations: [
    AppComponent,
    NavMenuComponent,
    AboutComponent,
    OsmMapComponent,
    HistoryChartComponent,
    PopupComponent,
    PredictionChartComponent,
    LegendComponent
  ],
  imports: [
    BrowserModule.withServerTransition({ appId: 'ng-cli-universal' }),
    HttpClientModule,
    FormsModule,
    RouterModule.forRoot([
      { path: '', component: OsmMapComponent, pathMatch: 'full' },
      { path: 'about', component: AboutComponent}
    ]),
    LoadingBarModule.forRoot(),
    NgbModule
  ],
  providers: [
    DustService,
    LoadingProgressService, {
      provide: HTTP_INTERCEPTORS,
      useClass: LoadingInterceptor,
      multi: true
    }],
  bootstrap: [AppComponent]
})
export class AppModule { }
