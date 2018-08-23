import { Component, OnInit, ViewChild, Inject } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { MapsAPILoader, AgmMap } from '@agm/core';

@Component({
  selector: 'app-gmap',
  templateUrl: './gmap.component.html',
  styleUrls: ['./gmap.component.css']
})
export class GMapComponent implements OnInit {
  @ViewChild(AgmMap) map: AgmMap;
  points: Point[];
  public location = {
    lat: 48.144677,
    lng: 17.109717,
    zoom: 5
  };

  constructor(private http: HttpClient,
    @Inject('BASE_URL') private baseUrl: string,
    mapsApiLoader: MapsAPILoader) {
    mapsApiLoader.load();
  }

  ngOnInit() {
    this.http.get<Point[]>(this.baseUrl + 'api/SampleData/Points').subscribe(result => {
      this.points = result;
    }, error => console.error(error));
  }
}

interface Point {
  Lat: number;
  Lon: number;
  Size: number;
  Timestamp: Date;
}
