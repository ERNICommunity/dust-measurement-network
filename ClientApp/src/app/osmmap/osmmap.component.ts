import { Component, OnInit } from '@angular/core';

import Map from 'ol/Map';
import View from 'ol/View';
import TileLayer from 'ol/layer/Tile';
// import XYZ from 'ol/source/XYZ';
import OSM from 'ol/source/OSM';
import { fromLonLat } from 'ol/proj';

@Component({
  selector: 'app-osmmap',
  templateUrl: './osmmap.component.html',
  styleUrls: ['./osmmap.component.css']
})
export class OsmMapComponent implements OnInit {
  map: Map;

  constructor() { }

  ngOnInit() {
    this.map = new Map({
      target: 'osmmap',
      layers: [
        new TileLayer({
          source: new OSM()
          // source: new XYZ({
          //   url: 'https://{a-c}.tile.openstreetmap.org/{z}/{x}/{y}.png'
          // })
        })
      ],
      view: new View({
        center: fromLonLat([17.109717, 48.144677]),
        zoom: 5
      })
    });
  }

}
