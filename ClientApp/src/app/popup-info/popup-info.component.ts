import { Component, OnInit } from '@angular/core';
import { DatePipe, DecimalPipe } from '@angular/common';

@Component({
  selector: 'app-popup-info',
  templateUrl: './popup-info.component.html',
  styleUrls: ['./popup-info.component.css']
})
export class PopupInfoComponent implements OnInit {

  constructor() { }

  ngOnInit() {
  }

  private formatTimestamp(stamp: Date) {
    const datePipe = new DatePipe('en-US');
    return datePipe.transform(stamp, 'long');
  }

}
