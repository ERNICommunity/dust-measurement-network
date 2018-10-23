import { Control } from 'ol/control';
import Map from 'ol/Map';
import { fromLonLat } from 'ol/proj';

export class UserLocation extends Control {
  constructor() {
    const button = document.createElement('button');
    button.title = 'My location';
    button.innerHTML = '&#9678;'; // utf8 bullseye

    const element = document.createElement('div');
    element.className = 'user-location ol-unselectable ol-control';
    element.appendChild(button);

    super({
      element: element
    });

    button.addEventListener('click', () => this.navigateMapToUsersPosition(super.getMap()), false);
    button.addEventListener('touchstart', () => this.navigateMapToUsersPosition(super.getMap()), false);
  }

  private navigateMapToUsersPosition(m: Map) {
    navigator.geolocation.getCurrentPosition(pos => {
      const coords = fromLonLat([pos.coords.longitude, pos.coords.latitude]);
      m.getView().animate({center: coords, zoom: 13});
    });
  }
}
