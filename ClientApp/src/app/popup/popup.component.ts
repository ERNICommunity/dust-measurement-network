import { Component, ViewChild } from '@angular/core';
import { NgbModal } from '@ng-bootstrap/ng-bootstrap';
import { SensorDto } from '../service/SensorDto';

@Component({
  selector: 'app-popup',
  templateUrl: './popup.component.html',
  styleUrls: ['./popup.component.css']
})
export class PopupComponent {
  @ViewChild('content') private modal;

  id: number;
  name: string;
  latitude: number;
  longitude: number;
  timestamp: string;
  particulateMatter25: number;
  particulateMatter100: number;

  constructor(private modalService: NgbModal) { }

  open(data: SensorDto) {
    this.id = data.id;
    this.name = data.name;
    this.latitude = data.latitude;
    this.longitude = data.longitude;
    this.timestamp = data.timestamp;
    this.particulateMatter25 = data.particulateMatter25;
    this.particulateMatter100 = data.particulateMatter100;
    this.modalService.open(this.modal);
  }
}
