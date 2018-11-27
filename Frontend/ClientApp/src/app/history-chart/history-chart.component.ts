import { Component, Input, OnInit, } from '@angular/core';
import { DustService } from '../service/dust.service';
import * as Chartist from 'chartist';

@Component({
  selector: 'app-history-chart',
  templateUrl: './history-chart.component.html',
  styleUrls: ['./history-chart.component.css']
})
export class HistoryChartComponent implements OnInit {
  private _id: number;
  private _dateFrom: Date;
  private _dateTo: Date;
  data = {
    labels: [],
    series: []
  };

  @Input() set id(value: number) {
    this._id = value;
  }

  set dateFrom(value: string) {
    this._dateFrom = new Date(value);
    this.updateDustData();
  }
  get dateFrom(): string {
    return this.formatForDateInput(this._dateFrom);
  }

  set dateTo(value: string) {
    this._dateTo = new Date(value);
    this.updateDustData();
  }
  get dateTo(): string {
    return this.formatForDateInput(this._dateTo);
  }

  constructor(private dustService: DustService) {
    const now = new Date();
    this._dateFrom = new Date(now.getFullYear(), now.getMonth(), now.getDate() - 20, 0, 0, 0, 0);
    this._dateTo = new Date(now.getFullYear(), now.getMonth(), now.getDate(), 0 , 0, 0, 0);
    this.data = { labels: [], series: [] };
  }

  ngOnInit(): void {
    this.updateDustData();
  }

  private formatForDateInput(date: Date) {
    const year = date.getFullYear();
    const month = date.getMonth() + 1;
    const day = date.getDate();
    return `${year}-${month < 10 ? '0' + month : month}-${day < 10 ? '0' + day : day}`;
  }

  private updateDustData(): void {
    this.dustService.getDustHistory(this._id, this._dateFrom, this._dateTo).subscribe(
      results => {
        const data = { labels: [], series: [] };
        data.series[0] = [];
        data.series[1] = [];
        for (const r of results) {
          data.labels.push(new Date(r.timestamp));
          data.series[0].push(r.particulateMatter25);
          data.series[1].push(r.particulateMatter100);
        }
        const ch = new Chartist.Line('.ct-chart', data);
      },
      err => console.error(err)
    );
  }
}
