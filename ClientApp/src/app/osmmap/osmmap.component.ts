import { Component, OnInit, Inject } from '@angular/core';
import { HttpClient } from '@angular/common/http';

import Map from 'ol/Map';
import View from 'ol/View';
import TileLayer from 'ol/layer/Tile';
import VectorLayer from 'ol/layer/Vector';
// import XYZ from 'ol/source/XYZ';
import OSMSource from 'ol/source/OSM';
import VectorSource from 'ol/source/Vector';
import { fromLonLat, transform } from 'ol/proj';
import Feature from 'ol/Feature';
import Point from 'ol/geom/Point';

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
          source: this.vectorSource
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
        geometry: new Point(transform([markers[i].Lon, markers[i].Lat], 'EPSG:4326', 'EPSG:3857')),
        name: 'Point' + i,
        timestamp: markers[i].Timestamp,
        size: markers[i].Size
      });
      this.vectorSource.addFeature(iconFeature);
    }
  }
}

interface Marker {
  Lat: number;
  Lon: number;
  Size: number;
  Timestamp: Date;
}
