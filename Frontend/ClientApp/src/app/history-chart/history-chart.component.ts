import { Component, Input, OnInit, } from '@angular/core';

import { BehaviorSubject, combineLatest } from 'rxjs';
import { switchMap } from 'rxjs/operators';

import {  IChartistData } from 'chartist';

import { DustService } from '../service/dust.service';

@Component({
  selector: 'app-history-chart',
  templateUrl: './history-chart.component.html',
  styleUrls: ['./history-chart.component.css']
})
export class HistoryChartComponent implements OnInit {
  private _id: number;
  private _dateFrom: BehaviorSubject<Date>;
  private _dateTo: BehaviorSubject<Date>;
  data: IChartistData;

  @Input() set id(value: number) {
    this._id = value;
  }

  set dateFrom(value: string) {
    this._dateFrom .next(new Date(value));
  }
  get dateFrom(): string {
    return this.formatForDateInput(this._dateFrom.value);
  }

  set dateTo(value: string) {
    this._dateTo.next(new Date(value));
  }
  get dateTo(): string {
    return this.formatForDateInput(this._dateTo.value);
  }

  constructor(private _dustService: DustService) {
    const now = new Date();
    this._dateFrom = new BehaviorSubject<Date>(new Date(now.getFullYear(), now.getMonth(), now.getDate() - 20, 0, 0, 0, 0));
    this._dateTo = new BehaviorSubject<Date>(new Date(now.getFullYear(), now.getMonth(), now.getDate(), 0 , 0, 0, 0));
  }

  ngOnInit(): void {
    combineLatest([this._dateFrom, this._dateTo]).pipe(
      switchMap(([from, to]) => this._dustService.getDustHistory(this._id, from, to))
    )
    .subscribe(results =>
        this.data = {
        labels: results.map(x => new Date(x.timestamp)),
        series: [
          results.map(x => x.particulateMatter25),
          results.map(x => x.particulateMatter100)
        ]
      },
      err => console.error(err)
    );
  }

  private formatForDateInput(date: Date) {
    const year = date.getFullYear();
    const month = date.getMonth() + 1;
    const day = date.getDate();
    return `${year}-${month < 10 ? '0' + month : month}-${day < 10 ? '0' + day : day}`;
  }
}
