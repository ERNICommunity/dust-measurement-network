import { Control } from 'ol/control';
import Map from 'ol/Map';

export class UserLocation extends Control {
  constructor(clickCallback: (m: Map) => void) {
    const button = document.createElement('button');
    button.title = 'My location';
    button.innerHTML = '&#9678;'; // utf8 bullseye

    const element = document.createElement('div');
    element.className = 'user-location ol-unselectable ol-control';
    element.appendChild(button);

    super({
      element: element
    });

    button.addEventListener('click', () => clickCallback(super.getMap()), false);
    button.addEventListener('touchstart', () => clickCallback(super.getMap()), false);
  }
}
