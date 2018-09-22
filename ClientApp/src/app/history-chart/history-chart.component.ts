import { Component, Input, OnInit } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { DatePipe } from '@angular/common';
import { Chart } from 'chart.js';
import { DustService } from '../service/dust.service';
import { DustDto } from '../service/DustDto';
import { forkJoin } from 'rxjs';
import * as moment from 'moment'

@Component({
  selector: 'history-chart',
  templateUrl: './history-chart.component.html',
  styleUrls: ['./history-chart.component.css']
})
export class HistoryChartComponent {
  private _id : number;
  private _dateFrom: Date;
  private _dateTo: Date; 

  @Input() set id(value: number) {
    this._id = value;
    this.dateFrom = this.dateFromStr;
    this.dateTo = this.dateToStr;
    this.updateDustData(this._dateFrom, this._dateTo);
  }

  dateFromStr: string = moment().subtract(2, 'days').startOf('day').format("YYYY-MM-DD");
  dateToStr: string = moment().endOf('day').format("YYYY-MM-DD");;

  set dateFrom(value: string){
    this._dateFrom = moment(this.dateFromStr).startOf('day').toDate();
  }

  set dateTo(value: string){
    this._dateTo = moment(this.dateToStr).endOf('day').toDate();
  }

  constructor(private dustService: DustService) {}

  dateRangeChanged() {
    this.dateFrom = this.dateFromStr;
    this.dateTo = this.dateToStr;
    
    this.updateDustData(this._dateFrom, this._dateTo);
  }

  private updateDustData(dateFrom: Date, dateTo: Date) : void {
    const datePipe = new DatePipe('en-US');
    this.dustService.getDustHistory(this._id, dateFrom, dateTo).subscribe(
      result => this.drawChart(result),
      err => console.error(err)
    );
  }

  private drawChart(history: DustDto[]) : void {
    const pipe = new DatePipe('en-US');
    const ctx = document.getElementById('chartcanvas') as HTMLCanvasElement;
    const chart = new Chart(ctx.getContext('2d'), {
      type: 'line',
      data: {
        labels: history.map(dustdata => pipe.transform(dustdata.timestamp, 'medium')),
        datasets: [
          {
            label: 'Dust 2.5',
            // backgroundColor: 'rgb(18, 99, 132)',
            borderColor: 'rgb(18, 99, 32)',
            data: (history.map(dustdata => dustdata.particulateMatter25))
          },
          {
            label: 'Dust 10.0',
            // backgroundColor: 'rgb(18, 3, 132)',
            borderColor: 'rgb(18, 3, 32)',
            data: (history.map(dustdata => dustdata.particulateMatter100))
          }]
      },
      options: {
        legend: {
          display: true
        },
        scales: {
          xAxes: [{
            display: true
          }],
          yAxes: [{
            display: true,
            ticks: {
              min: 0
            }
          }],
        }
      }
    });
  }
}
