import { Component, AfterViewInit, OnInit, Inject } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { DatePipe, DecimalPipe } from '@angular/common';

import Map from 'ol/Map';
import View from 'ol/View';
import TileLayer from 'ol/layer/Tile';
import VectorLayer from 'ol/layer/Vector';
// import XYZ from 'ol/source/XYZ';
import OSMSource from 'ol/source/OSM';
import VectorSource from 'ol/source/Vector';
import { fromLonLat } from 'ol/proj';
import Feature from 'ol/Feature';
import Point from 'ol/geom/Point';
import Overlay from 'ol/Overlay';
import { Circle, Fill, Stroke, Style } from 'ol/style';

@Component({
  selector: 'app-osmmap',
  templateUrl: './osmmap.component.html',
  styleUrls: ['./osmmap.component.css']
})
export class OsmMapComponent implements AfterViewInit, OnInit {
  private map: Map;
  private defaultLatitude: number = 47.3769;
  private defaultLongitude: number = 8.5417;
  private vectorSource = new VectorSource();
  isLoaded: boolean;
  private overlay: Overlay;

  constructor(private http: HttpClient, @Inject('BASE_URL') private baseUrl: string) {
    this.isLoaded = false;
   }

  ngAfterViewInit() {
    this.trackPosition();

    var closer = document.getElementById('popup-closer');
    closer.onclick = function() {
      this.overlay.setPosition(undefined);
      closer.blur();
      return false;
    }.bind(this);
  }

  ngOnInit() {
    /**
     * Elements that make up the popup.
     */
    var container = document.getElementById('popup');
    this.overlay = new Overlay({
      element: container,
      autoPan: true,
      autoPanAnimation: {
        duration: 250
      }
    });

    this.map = new Map({
      target: 'osmmap',
      overlays: [this.overlay],
      layers: [
        new TileLayer({
          source: new OSMSource()
        }),
        new VectorLayer({
          source: this.vectorSource,
          style: new Style({
            fill: new Fill({
              color: '#000000'
            }),
            stroke: new Stroke({
              color: '#ffffff',
              width: 2
            }),
            image: new Circle({
              radius: 5,
              fill: new Fill({
                color: '#000000'
              })
            })
          })
        })
      ],
      view: new View({
        center: fromLonLat([this.defaultLongitude, this.defaultLatitude]),
        zoom: 13
      })
    });

    this.map.on('click', (evt) => {
      var f = this.map.forEachFeatureAtPixel(
          evt.pixel,
          function(ft, layer){return ft;}
      );
      if (f) {
          var geometry = f.getGeometry();
          var coord = geometry.getCoordinates();
          
          document.getElementById('popup-content').innerHTML = 
          `<p>
          <strong>ID:</strong> ${f.get('name')}<br>
          <strong>PM2.5:</strong> ${new DecimalPipe('en-US').transform(f.get('particulateMatter25'))} µg/m<sup>3</sup><br>
          <strong>PM10:</strong> ${new DecimalPipe('en-US').transform(f.get('particulateMatter100'))} µg/m<sup>3</sup><br>
          <strong>Last measurement:</strong> ${this.formatTimestamp(f.get('timestamp'))}
          </p>`;
          this.overlay.setPosition(coord);
      }
    });

    this.http.get<Marker[]>(this.baseUrl + 'api/sensors').subscribe(
      result => this.render(result),
      error => console.error(error));
  }

  private render(markers: Marker[]) {
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

  private trackPosition() {
    if(navigator.geolocation) {
      var view = this.map.getView();
      navigator.geolocation.getCurrentPosition((position) => {
        view.setCenter(fromLonLat([position.coords.longitude, position.coords.latitude]));
        view.setZoom(13);
        this.isLoaded = true;
      });
    } else {
      this.isLoaded = true;
    }
  }

  private formatTimestamp(stamp: Date) {
    const datePipe = new DatePipe('en-US');
    return datePipe.transform(stamp, 'long');
  }
}

interface Marker {
  id: number;
  lat: number;
  lon: number;
  timestamp: Date;
  particulateMatter25: number;
  particulateMatter100: number;
}
