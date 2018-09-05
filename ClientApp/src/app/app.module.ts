import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpClientModule, HTTP_INTERCEPTORS } from '@angular/common/http';
import { RouterModule } from '@angular/router';

import { AppComponent } from './app.component';
import { NavMenuComponent } from './nav-menu/nav-menu.component';
import { AboutComponent } from './about/about.component';
import { OsmMapComponent } from './osmmap/osmmap.component';
import { ChartComponent } from './chart/chart.component';
import { DustService } from './service/dust.service';
import { PopupInfoComponent } from './popup-info/popup-info.component';
import { LoadingProgressService } from './service/loading-progress.service';
import { LoadingInterceptor } from './service/loading.interceptor';

@NgModule({
  declarations: [
    AppComponent,
    NavMenuComponent,
    AboutComponent,
    OsmMapComponent,
    ChartComponent,
    PopupInfoComponent
  ],
  imports: [
    BrowserModule.withServerTransition({ appId: 'ng-cli-universal' }),
    HttpClientModule,
    FormsModule,
    RouterModule.forRoot([
      { path: '', component: OsmMapComponent, pathMatch: 'full' },
      { path: 'about', component: AboutComponent},
      { path: 'chart/:id', component: ChartComponent }
    ])
  ],
  entryComponents: [PopupInfoComponent],
  providers: [
    DustService,
    LoadingProgressService,
    {
      provide: HTTP_INTERCEPTORS,
      useClass: LoadingInterceptor,
      multi: true
    }],
  bootstrap: [AppComponent]
})
export class AppModule { }
