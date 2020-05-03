import { Component, OnInit, OnDestroy, ViewChild } from '@angular/core';

import Map from 'ol/Map';
import View from 'ol/View';
import TileLayer from 'ol/layer/Tile';
import VectorLayer from 'ol/layer/Vector';
import OSM from 'ol/source/OSM';
import VectorSource from 'ol/source/Vector';
import Cluster from 'ol/source/Cluster';
import { defaults as defaultControls, OverviewMap, Attribution, ScaleLine } from 'ol/control';
import { fromLonLat, transformExtent } from 'ol/proj';
import Feature from 'ol/Feature';
import Geolocation from 'ol/Geolocation';
import Point from 'ol/geom/Point';
import MapEvent from 'ol/MapEvent';
import MapBrowserEvent from 'ol/MapBrowserEvent';
import { Circle, Fill, Stroke, Style, Text } from 'ol/style';

import { Subscription, fromEvent, timer, EMPTY } from 'rxjs';
import { debounceTime, map, switchMap, catchError } from 'rxjs/operators';

import { SensorDto } from '../service/SensorDto';
import { DustService } from '../service/dust.service';
import { PopupComponent } from '../popup/popup.component';
import { UserLocation } from './user-location.class';

@Component({
  selector: 'app-osmmap',
  templateUrl: './osmmap.component.html',
  styleUrls: ['./osmmap.component.css']
})
export class OsmMapComponent implements OnInit, OnDestroy {
  private _sensorsVectorSource = new VectorSource();
  private _positionFeature = new Feature();
  private _mapMoveSubscription: Subscription;
  @ViewChild(PopupComponent) private _popup: PopupComponent;

  constructor(private _dustService: DustService) {}

  ngOnInit() {
    const osmMap = new Map({
      target: 'osmmap',
      layers: [
        new TileLayer({
          source: new OSM()
        }),
        new VectorLayer({
          source: new VectorSource({
            features: [this._positionFeature]
          })
        }),
        new VectorLayer({
          source: new Cluster({
            attributions: '© Dust data by <a href="https://www.betterask.erni/" target="_blank">ERNI</a> Community',
            source: this._sensorsVectorSource,
            distance: 30
          }),
          style: f => this.getStyle(f)
        })
      ],
      view: new View({
        center: [0, 0],
        zoom: 0
      }),
      controls: defaultControls({attribution: false}).extend([
        new Attribution({collapsible: true}),
        new ScaleLine(),
        new OverviewMap(),
        new UserLocation(m =>  this.navigateMapToUsersPosition(m))
      ])
    });

    const geolocation = new Geolocation({
      tracking: true,
      trackingOptions: { enableHighAccuracy: true, timeout: 2000 },
      projection: osmMap.getView().getProjection()
    });
    geolocation.on('error', err => console.error('geolocation error', err));
    geolocation.on('change', evt => this._positionFeature.setGeometry(evt.target.getAccuracyGeometry()));
    geolocation.once('change', evt => osmMap.getView().animate({center: evt.target.getPosition(), zoom: 10}));

    osmMap.on('click', (evt: MapBrowserEvent) => {
      const features = evt.map.forEachFeatureAtPixel(evt.pixel, (ft, layer) => ft);
      if (features && features.get('features').length === 1) {
        this._popup.open(features.get('features')[0].get('data'));
      }
    });

    this._mapMoveSubscription = fromEvent(osmMap, 'moveend').pipe(
      debounceTime(1000),
      map((evt: MapEvent) => transformExtent(evt.frameState.extent, evt.map.getView().getProjection(), 'EPSG:4326')),
      switchMap(extent => timer(0, 5000).pipe( // emit immediatelly, then every 5s
        map(_ => extent)
      )),
      switchMap(extent => this._dustService.getSensors(extent[0], extent[1], extent[2], extent[3]).pipe(
        catchError(e => {
          console.warn('getting sensors failed', e);
          return EMPTY; // if call to server fails just ignore it
        })
      )),
    ).subscribe(
      result => this.drawMarkers(result),
      err => console.error('mapMoveSubscription fail', err)
    );
  }

  ngOnDestroy(): void {
    this._mapMoveSubscription.unsubscribe();
  }

  private getStyle(feature: Feature) {
    const features = feature.get('features');
    if (features.length === 1) {
      const data = (features[0].get('data') as SensorDto);
      const matter25 = data.particulateMatter25;
      const matter100 = data.particulateMatter100;
      return [
        matter25 ? new Style({
          image: new Circle({
            radius: 20,
            stroke: new Stroke({
              color: this.getColor25(matter25),
              width: 12.5
            })
          }),
          text: new Text({
            text: matter25 && matter25.toFixed(1),
            scale: 1.2,
            offsetY: 20
          })
        }) : new Style(),
          new Style({
          image: new Circle({
            radius: 12.5,
            fill: new Fill({
              color: this.getColor100(matter100),
            })
          }),
          text: new Text({
            text: matter100 && matter100.toFixed(1),
            scale: 1.2
          })
        })
      ];
    } else {
      return [
        new Style({
          image: new Circle({
            radius: 20,
            stroke: new Stroke({
              color: this.getColor25(this.getAverage(features as Feature[], 'particulateMatter25')),
              width: 12.5
            })
          }),
        }),
        new Style({
          image: new Circle({
            radius: 12.5,
            fill: new Fill({
              color: this.getColor100(this.getAverage(features as Feature[], 'particulateMatter100')),
            })
          }),
          text: new Text({
            text: features.length.toString(),
            scale: 1.7
          })
        })
      ];
    }
  }

  private drawMarkers(dtos: SensorDto[]) {
    this._sensorsVectorSource.clear();
    for (const dto of dtos) {
      const sensorFeature = new Feature({
        geometry: new Point(fromLonLat([dto.longitude, dto.latitude])),
        name: dto.name,
        data: dto
      });
      this._sensorsVectorSource.addFeature(sensorFeature);
    }
  }

  private navigateMapToUsersPosition(m: Map) {
    m.getView().fit(this._positionFeature.getGeometry(), { duration: 1000 });
  }

  private getColor25(matterDensity: number) {
    if (!matterDensity) {
      return [0, 0, 0, 0.5];
    }
    switch (true) {
      case matterDensity < 25:
        return [0, 255, 0, 0.5];
      case matterDensity < 50:
        return [255, 255, 0, 0.5];
      case matterDensity >= 50:
        return [255, 0, 0, 0.5];
    }
  }

  private getColor100(matterDensity: number) {
    if (!matterDensity) {
      return [0, 0, 0, 0.5];
    }
    switch (true) {
      case matterDensity < 50:
        return [0, 255, 0, 0.5];
      case matterDensity < 100:
        return [255, 255, 0, 0.5];
      case matterDensity >= 100:
        return [255, 0, 0, 0.5];
    }
  }

  private getAverage(matterDensities: Feature[], key: string): number {
    return matterDensities.reduce((a, b) => a + (b.get('data') as SensorDto)[key], 0) / matterDensities.length;
  }
}
