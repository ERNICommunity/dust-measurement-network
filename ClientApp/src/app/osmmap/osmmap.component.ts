import { Component, OnInit, Inject } from '@angular/core';
import { HttpClient } from '@angular/common/http';

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
import { Circle, Fill, Stroke, Style } from 'ol/style';

@Component({
  selector: 'app-osmmap',
  templateUrl: './osmmap.component.html',
  styleUrls: ['./osmmap.component.css']
})
export class OsmMapComponent implements OnInit {
  private map: Map;
  private vectorSource = new VectorSource();

  constructor(private http: HttpClient, @Inject('BASE_URL') private baseUrl: string) { }

  ngOnInit() {
    this.map = new Map({
      target: 'osmmap',
      layers: [
        new TileLayer({
          source: new OSMSource()
          // source: new XYZ({
          //   url: 'https://{a-c}.tile.openstreetmap.org/{z}/{x}/{y}.png'
          // })
        }),
        new VectorLayer({
          source: this.vectorSource,
          style: new Style({
            fill: new Fill({
              color: 'rgba(255, 255, 255, 0.2)'
            }),
            stroke: new Stroke({
              color: '#ffffff',
              width: 2
            }),
            image: new Circle({
              radius: 7,
              fill: new Fill({
                color: '#ffcc33'
              })
            })
          })
        })
      ],
      view: new View({
        center: fromLonLat([17.109717, 48.144677]),
        zoom: 5
      })
    });
    this.http.get<Marker[]>(this.baseUrl + 'api/SampleData/Points').subscribe(
      result => this.render(result),
      error => console.error(error));
  }

  private render(markers: Marker[]) {
    this.vectorSource.clear();
    for (let i = 0; i < markers.length; i++) {
      const iconFeature = new Feature({
        geometry: new Point(fromLonLat([markers[i].lon, markers[i].lat])),
        name: 'Point' + i,
        timestamp: markers[i].timestamp,
        size: markers[i].size
      });
      this.vectorSource.addFeature(iconFeature);
    }
  }
}

interface Marker {
  lat: number;
  lon: number;
  size: number;
  timestamp: Date;
}
