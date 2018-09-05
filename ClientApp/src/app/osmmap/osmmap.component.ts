import { Component, AfterViewInit, OnInit, OnDestroy, ApplicationRef, ComponentFactoryResolver, Injector, EmbeddedViewRef } from '@angular/core';

import Map from 'ol/Map';
import View from 'ol/View';
import TileLayer from 'ol/layer/Tile';
import VectorLayer from 'ol/layer/Vector';
import OSMSource from 'ol/source/OSM';
import VectorSource from 'ol/source/Vector';
import { fromLonLat, transformExtent } from 'ol/proj';
import Feature from 'ol/Feature';
import Point from 'ol/geom/Point';
import Overlay from 'ol/Overlay';
import MapEvent from 'ol/MapEvent';
import { Circle, Fill, Stroke, Style } from 'ol/style';

import { Subscription, fromEvent } from 'rxjs';
import { debounceTime, map, switchMap } from 'rxjs/operators';

import { PopupInfoComponent } from '../popup-info/popup-info.component';
import { SensorDto } from '../service/SensorDto';
import { DustService } from '../service/dust.service';

@Component({
  selector: 'app-osmmap',
  templateUrl: './osmmap.component.html',
  styleUrls: ['./osmmap.component.css']
})
export class OsmMapComponent implements AfterViewInit, OnInit, OnDestroy {
  private map: Map;
  private vectorSource = new VectorSource();
  private mapMoveSubscription: Subscription;

  constructor(
    private dustService: DustService,
    private appRef: ApplicationRef,
    private componentFactoryResolver: ComponentFactoryResolver,
    private injector: Injector
  ) {}

  ngAfterViewInit() {
    const closer = document.getElementById('popup-closer');
    closer.onclick = function() {
      this.overlay.setPosition(undefined);
      closer.blur();
      return false;
    }.bind(this);
  }

  ngOnInit() {
    const overlay = new Overlay({
      element: document.getElementById('popup'),
      autoPan: true,
      autoPanAnimation: {
        duration: 250
      }
    });

    this.map = new Map({
      target: 'osmmap',
      overlays: [overlay],
      layers: [
        new TileLayer({
          source: new OSMSource()
        }),
        new VectorLayer({
          source: this.vectorSource,
          style: new Style({
            image: new Circle({
              radius: 8,
              fill: new Fill({
                color: [34, 34, 34, 0.9]
              }),
              stroke: new Stroke({
                color: [157, 157, 157, 0.9],
                width: 1
              })
            })
          })
        })
      ],
      view: new View({
        center: [0, 0],
        zoom: 13
      })
    });

    this.map.on('click', evt => {
      const features = this.map.forEachFeatureAtPixel(evt.pixel, (ft, layer) => ft);
      if (features) {
          const geometry = features.getGeometry();
          const coord = geometry.getCoordinates();
          this.appendPopup(features);
          overlay.setPosition(coord);
      }
    });

    this.mapMoveSubscription = fromEvent(this.map, 'moveend').pipe(
      debounceTime(1000),
      map((evt: MapEvent) => transformExtent(evt.frameState.extent, 'EPSG:3857', 'EPSG:4326')),
      switchMap(extent => this.dustService.getSensors(extent[0], extent[1], extent[2], extent[3]))
    ).subscribe(
      result => this.drawMarkers(result),
      err => console.error(err)
    );
    this.getUserPosition();
  }

  ngOnDestroy(): void {
   this.mapMoveSubscription.unsubscribe();
  }

  private drawMarkers(markers: SensorDto[]) {
    this.vectorSource.clear();
    for (let i = 0; i < markers.length; i++) {
      const iconFeature = new Feature({
        geometry: new Point(fromLonLat([markers[i].lon, markers[i].lat])),
        name: markers[i].id,
        timestamp: markers[i].timestamp,
        particulateMatter25: markers[i].particulateMatter25,
        particulateMatter100: markers[i].particulateMatter100
      });
      this.vectorSource.addFeature(iconFeature);
    }
  }

  private getUserPosition() {
    if (navigator.geolocation) {
      const view = this.map.getView();
      navigator.geolocation.getCurrentPosition(position => {
        view.setCenter(fromLonLat([position.coords.longitude, position.coords.latitude]));
        view.setZoom(13);
      });
    }
  }

  private appendPopup(sensorData: any) {
    const popupContent = document.getElementById('popup-content');
    const nodeList = popupContent.childNodes;
    for (let i = 0; i < nodeList.length; i++) {
      nodeList[i].remove();
    }

    const componentRef =
      this.componentFactoryResolver
      .resolveComponentFactory(PopupInfoComponent)
      .create(this.injector);

    this.appRef.attachView(componentRef.hostView);

    const domElem = (componentRef.hostView as EmbeddedViewRef<any>)
      .rootNodes[0] as HTMLElement;
    (<PopupInfoComponent>componentRef.instance).data = {
      id: sensorData.get('name'),
      particulateMatter25: sensorData.get('particulateMatter25'),
      particulateMatter100: sensorData.get('particulateMatter100'),
      timestamp: sensorData.get('timestamp')
    };

    popupContent.appendChild(domElem);
  }
}
