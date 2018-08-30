import { Component, OnInit, Input } from '@angular/core';
import { DatePipe, DecimalPipe } from '@angular/common';

@Component({
  selector: 'app-popup-info',
  templateUrl: './popup-info.component.html',
  styleUrls: ['./popup-info.component.css']
})
export class PopupInfoComponent implements OnInit {
  @Input() data: PopupData;

  constructor() { }

  ngOnInit() {
  }

  formatTimestamp(stamp: Date) {
    const datePipe = new DatePipe('en-US');
    return datePipe.transform(stamp, 'long');
  }
}

interface PopupData {
  id: number;
  particulateMatter25: number;
  particulateMatter100: number;
  timestamp: Date;
}
