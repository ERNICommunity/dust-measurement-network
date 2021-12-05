import { BrowserModule } from '@angular/platform-browser';
import { NgModule, APP_INITIALIZER } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpClientModule, HTTP_INTERCEPTORS, HttpClient } from '@angular/common/http';
import { RouterModule } from '@angular/router';

import { LoadingBarModule } from '@ngx-loading-bar/core';
import { NgbModule } from '@ng-bootstrap/ng-bootstrap';
import { ChartistModule } from 'ng-chartist';
import { FontAwesomeModule } from '@fortawesome/angular-fontawesome';

import { AppComponent } from './app.component';
import { NavMenuComponent } from './nav-menu/nav-menu.component';
import { AboutComponent } from './about/about.component';
import { OsmMapComponent } from './osmmap/osmmap.component';
import { HistoryChartComponent } from './history-chart/history-chart.component';
import { DustService } from './service/dust.service';
import { RunningRequestService } from './service/running-request.service';
import { LoadingInterceptor } from './service/loading.interceptor';
import { PopupComponent } from './popup/popup.component';
import { PredictionChartComponent } from './prediction-chart/prediction-chart.component';
import { LegendComponent } from './legend/legend.component';
import { ConfigService } from './service/config.service';

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
      { path: 'about', component: AboutComponent }
    ]),
    LoadingBarModule,
    ChartistModule,
    NgbModule,
    FontAwesomeModule
  ],
  providers: [
    DustService,
    RunningRequestService,
    ConfigService, {
      provide: HTTP_INTERCEPTORS,
      useClass: LoadingInterceptor,
      multi: true
    }, {
      provide: APP_INITIALIZER,
      useFactory: ConfigService.factory,
      deps: [HttpClient, 'BASE_URL'],
      multi: true
    }],
  bootstrap: [AppComponent]
})
export class AppModule { }
