import { Component, OnInit, OnDestroy, ViewChild } from '@angular/core';

import Map from 'ol/Map';
import View from 'ol/View';
import TileLayer from 'ol/layer/Tile';
import VectorLayer from 'ol/layer/Vector';
import OSM, {ATTRIBUTION} from 'ol/source/OSM';
import VectorSource from 'ol/source/Vector';
import Cluster from 'ol/source/Cluster';
import { defaults as defaultControls, OverviewMap } from 'ol/control';
import { fromLonLat, transformExtent } from 'ol/proj';
import Feature from 'ol/Feature';
import Point from 'ol/geom/Point';
import MapEvent from 'ol/MapEvent';
import { Circle, Fill, Stroke, Style, Text } from 'ol/style';

import { Subscription, fromEvent } from 'rxjs';
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
  private vectorSource = new VectorSource();
  private mapMoveSubscription: Subscription;
  @ViewChild(PopupComponent) private popup: PopupComponent;

  constructor(private dustService: DustService) {}

  ngOnInit() {
    const osmMap = new Map({
      target: 'osmmap',
      layers: [
        new TileLayer({
          source: new OSM({
            attributions: ['© <a href="https://www.betterask.erni/">ERNI</a> Community', ATTRIBUTION]
          })
        }),
        new VectorLayer({
          source: new Cluster({
            source: this.vectorSource,
            distance: 30
          }),
          style: f => this.getStyle(f)
        })
      ],
      view: new View({
        center: [0, 0],
        zoom: 13
      }),
      controls: defaultControls().extend([
        new OverviewMap(),
        new UserLocation(this.navigateMapToUsersPosition)
      ]),
    });

    osmMap.on('click', evt => {
      const features = evt.map.forEachFeatureAtPixel(evt.pixel, (ft, layer) => ft);
      if (features && features.get('features').length === 1) {
        this.popup.open(features.get('features')[0].get('data'));
      }
    });

    this.mapMoveSubscription = fromEvent(osmMap, 'moveend').pipe(
      debounceTime(1000),
      map((evt: MapEvent) => transformExtent(evt.frameState.extent, evt.map.getView().getProjection(), 'EPSG:4326')),
      switchMap(extent => this.dustService.getSensors(extent[0], extent[1], extent[2], extent[3])),
      catchError((err, source) => {
        console.error('getting available sensors failed', err);
        return source;
      })
    ).subscribe(
      result => this.drawMarkers(result),
      err => console.error('mapMoveSubscription fail', err)
    );

    this.navigateMapToUsersPosition(osmMap);
  }

  ngOnDestroy(): void {
   this.mapMoveSubscription.unsubscribe();
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

  private drawMarkers(markers: SensorDto[]) {
    this.vectorSource.clear();
    for (let i = 0; i < markers.length; i++) {
      const iconFeature = new Feature({
        geometry: new Point(fromLonLat([markers[i].longitude, markers[i].latitude])),
        name: markers[i].name,
        data: markers[i]
      });
      this.vectorSource.addFeature(iconFeature);
    }
  }

  private navigateMapToUsersPosition(m: Map) {
    navigator.geolocation.getCurrentPosition((pos: Position) => {
      const coords = fromLonLat([pos.coords.longitude, pos.coords.latitude]);
      m.getView().animate({center: coords, zoom: 13});
    });
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
