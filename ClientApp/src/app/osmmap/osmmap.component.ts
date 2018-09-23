import { Component, OnInit, OnDestroy, ViewChild } from '@angular/core';

import Map from 'ol/Map';
import View from 'ol/View';
import TileLayer from 'ol/layer/Tile';
import VectorLayer from 'ol/layer/Vector';
import OSMSource from 'ol/source/OSM';
import VectorSource from 'ol/source/Vector';
import { fromLonLat, transformExtent } from 'ol/proj';
import Feature from 'ol/Feature';
import Point from 'ol/geom/Point';
import MapEvent from 'ol/MapEvent';
import { Circle, Fill, Stroke, Style, Text } from 'ol/style';

import { Subscription, fromEvent } from 'rxjs';
import { debounceTime, map, switchMap } from 'rxjs/operators';

import { SensorDto } from '../service/SensorDto';
import { DustService } from '../service/dust.service';
import { PopupComponent } from '../popup/popup.component';

@Component({
  selector: 'app-osmmap',
  templateUrl: './osmmap.component.html',
  styleUrls: ['./osmmap.component.css']
})
export class OsmMapComponent implements OnInit, OnDestroy {
  private map: Map;
  private vectorSource = new VectorSource();
  private mapMoveSubscription: Subscription;
  @ViewChild(PopupComponent) private popup: PopupComponent;

  constructor(private dustService: DustService) {}

  ngOnInit() {
    this.map = new Map({
      target: 'osmmap',
      layers: [
        new TileLayer({
          source: new OSMSource()
        }), // add map control/overlay taha allows to get back to users location
        new VectorLayer({
          source: this.vectorSource,
          style: feature => {
            const featureData = feature.get('data') as SensorDto;
            const matter25 = featureData.particulateMatter25;
            const matter100 = featureData.particulateMatter100;
            if (!matter100 && !matter25) {
              return new Style({
                image: new Circle({
                  radius: 10,
                  fill: new Fill({
                    color: [0, 0, 0, 0.5]
                  })
                })
              });
            }
            return [
              matter25 ? new Style({
                image: new Circle({
                  radius: 25,
                  stroke: new Stroke({
                    color: this.getColor(matter25),
                    width: 15
                  })
                }),
                text: new Text({
                  text: matter25 && matter25.toFixed(1),
                  scale: 1.5,
                  offsetY: 25
                })
              }) : new Style(),
              matter100 ? new Style({
                image: new Circle({
                  radius: 15,
                  fill: new Fill({
                    color: this.getColor(matter100),
                  })
                }),
                text: new Text({
                  text: matter100 && matter100.toFixed(1),
                  scale: 1.5
                })
              }) : new Style()
            ];
          }
        })
      ],
      view: new View({
        center: [0, 0],
        zoom: 13
      })
    });

    this.getUserPosition();

    this.map.on('click', evt => {
      const feature = this.map.forEachFeatureAtPixel(evt.pixel, (ft, layer) => ft);
      if (feature) {
        this.popup.open(feature.get('data'));
      }
    });

    this.mapMoveSubscription = fromEvent(this.map, 'moveend').pipe(
      debounceTime(1000),
      map((evt: MapEvent) => transformExtent(evt.frameState.extent, this.map.getView().getProjection(), 'EPSG:4326')),
      switchMap(extent => this.dustService.getSensors(extent[0], extent[1], extent[2], extent[3]))
    ).subscribe(
      result => this.drawMarkers(result),
      err => console.error(err)
    );
  }

  ngOnDestroy(): void {
   this.mapMoveSubscription.unsubscribe();
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

  private getUserPosition() {
    navigator.geolocation.getCurrentPosition(pos => {
      const coords = fromLonLat([pos.coords.longitude, pos.coords.latitude]);
      this.map.getView().animate({center: coords, zoom: 13});
    });
  }

  private getColor(matterDensity: number) {
    switch (true) {
      case matterDensity < 25:
        return [0, 255, 0, 0.5];
      case matterDensity < 50:
        return [255, 255, 0, 0.5];
      case matterDensity >= 50:
        return [255, 0, 0, 0.5];
    }
  }
}
