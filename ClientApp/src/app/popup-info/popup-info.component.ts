import { Component, Input } from '@angular/core';

@Component({
  selector: 'app-popup-info',
  templateUrl: './popup-info.component.html',
  styleUrls: ['./popup-info.component.css']
})
export class PopupInfoComponent {
  @Input() data: PopupData;

  constructor() { }
}

interface PopupData {
  id: number;
  particulateMatter25: number;
  particulateMatter100: number;
  timestamp: Date;
}
