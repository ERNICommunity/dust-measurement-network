import { Component, Input, ViewChild,  } from '@angular/core';
import { DatePipe } from '@angular/common';
import { Chart } from 'chart.js';
import { DustService } from '../service/dust.service';
import { DustDto } from '../service/DustDto';
import * as moment from 'moment';

@Component({
  selector: 'app-history-chart',
  templateUrl: './history-chart.component.html',
  styleUrls: ['./history-chart.component.css']
})
export class HistoryChartComponent {
  private _id: number;
  private _dateFrom: Date;
  private _dateTo: Date;

  @ViewChild('canvas') elementView;

  @Input() set id(value: number) {
    this._id = value;
    this.dateFrom = this.dateFromStr;
    this.dateTo = this.dateToStr;
    this.updateDustData(this._dateFrom, this._dateTo);
  }

  dateFromStr: string = moment().subtract(2, 'days').startOf('day').format('YYYY-MM-DD');
  dateToStr: string = moment().endOf('day').format('YYYY-MM-DD');

  set dateFrom(value: string) {
    this._dateFrom = moment(this.dateFromStr).startOf('day').toDate();
  }

  set dateTo(value: string) {
    this._dateTo = moment(this.dateToStr).endOf('day').toDate();
  }

  constructor(private dustService: DustService) {}

  dateRangeChanged() {
    this.dateFrom = this.dateFromStr;
    this.dateTo = this.dateToStr;

    this.updateDustData(this._dateFrom, this._dateTo);
  }

  private updateDustData(dateFrom: Date, dateTo: Date): void {
    this.dustService.getDustHistory(this._id, dateFrom, dateTo).subscribe(
      result => this.drawChart(result),
      err => console.error(err)
    );
  }

  private drawChart(history: DustDto[]): void {
    const pipe = new DatePipe('en-US');
    const canvas = document.getElementById('chartcanvas') as HTMLCanvasElement;
    const context = canvas.getContext('2d');
    
    const green_red_gradient_25 = context.createLinearGradient(0, 400, 0, 0);
    green_red_gradient_25.addColorStop(0, 'green');
    green_red_gradient_25.addColorStop(0.25, 'yellow');
    green_red_gradient_25.addColorStop(0.5, 'red');

    const green_red_gradient_100 = context.createLinearGradient(0, 400, 0, 0);
    green_red_gradient_100.addColorStop(0, 'green');
    green_red_gradient_100.addColorStop(0.5, 'yellow');
    green_red_gradient_100.addColorStop(1, 'red');

    const chart = new Chart(context, {
      type: 'line',
      data: {
        labels: history.map(dustdata => pipe.transform(dustdata.timestamp, 'medium')),
        datasets: [
          {
            label: 'Dust 2.5',
            data: (history.map(dustdata => dustdata.particulateMatter25)),
            fill: false,
            borderColor: 'grey',
            pointBorderColor: green_red_gradient_25,
            pointBackgroundColor: green_red_gradient_25,
            pointHoverBackgroundColor: green_red_gradient_25,
            pointHoverBorderColor: green_red_gradient_25,
            pointRadius: 5,
            pointHoverRadius: 8
          },
          {
            label: 'Dust 10',
            data: (history.map(dustdata => dustdata.particulateMatter100)),
            fill: false,
            borderColor: 'darkgreen',
            pointBorderColor: green_red_gradient_100,
            pointBackgroundColor: green_red_gradient_100,
            pointHoverBackgroundColor: green_red_gradient_100,
            pointHoverBorderColor: green_red_gradient_100,
            pointRadius: 5,
            pointHoverRadius: 8
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
              max: 100,
              min: 0
            }
          }],
        }
      }
    });
  }
}
