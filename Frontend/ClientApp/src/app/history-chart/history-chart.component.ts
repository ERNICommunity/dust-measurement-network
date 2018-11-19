import { Component, Input, OnInit, } from '@angular/core';
import { Chart } from 'chart.js';
import { DustService } from '../service/dust.service';
import { DustDto } from '../service/DustDto';

@Component({
  selector: 'app-history-chart',
  templateUrl: './history-chart.component.html',
  styleUrls: ['./history-chart.component.css']
})
export class HistoryChartComponent implements OnInit {
  private _id: number;
  private _dateFrom: Date;
  private _dateTo: Date;

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
      result => this.drawChart(result),
      err => console.error(err)
    );
  }

  private drawChart(history: DustDto[]): void {
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
        labels: history.map(dustdata => new Date(dustdata.timestamp).toLocaleString()),
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
