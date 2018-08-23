import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';
import { RouterModule } from '@angular/router';

import { AppComponent } from './app.component';
import { NavMenuComponent } from './nav-menu/nav-menu.component';
import { HomeComponent } from './home/home.component';
import { GMapComponent } from './gmap/gmap.component';
import { AgmCoreModule, GoogleMapsAPIWrapper } from '@agm/core';
import { OsmMapComponent } from './osmmap/osmmap.component';

@NgModule({
  declarations: [
    AppComponent,
    NavMenuComponent,
    HomeComponent,
    GMapComponent,
    OsmMapComponent
  ],
  imports: [
    BrowserModule.withServerTransition({ appId: 'ng-cli-universal' }),
    HttpClientModule,
    FormsModule,
    AgmCoreModule.forRoot({apiKey: 'AIzaSyCr2awIpuQ-6lPU6QHLsN0g_ohnXz7g2gE'}),
    RouterModule.forRoot([
      { path: '', component: HomeComponent, pathMatch: 'full' },
      { path: 'gmap', component: GMapComponent },
      { path: 'osmmap', component: OsmMapComponent }
    ])
  ],
  providers: [
    GoogleMapsAPIWrapper
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
