import { Component, OnInit, Input } from '@angular/core';
import { DustService } from '../service/dust.service';
import { DustDto } from '../service/DustDto';
import { DatePipe } from '@angular/common';
import { Chart } from 'chart.js';

@Component({
  selector: 'prediction-chart',
  templateUrl: './prediction-chart.component.html',
  styleUrls: ['./prediction-chart.component.css']
})
export class PredictionChartComponent {
  @Input() set id(value: number) {
    this.updateDustData(value);
  }

  constructor(private dustService: DustService) { }

  private updateDustData(id: number) : void {
    this.dustService.getDustPrediction(id).subscribe(
      result => this.drawChart(result),
      err => console.error(err)
    );
  }

  private drawChart(prediction: DustDto[]) : void {
    const pipe = new DatePipe('en-US');
    const ctx = document.getElementById('canvas') as HTMLCanvasElement;
    const chart = new Chart(ctx.getContext('2d'), {
      type: 'line',
      data: {
        labels: prediction.map(dustdata => pipe.transform(dustdata.timestamp, 'medium')),
        datasets: [
          {
            label: 'Dust 2.5',
            // backgroundColor: 'rgb(18, 99, 132)',
            borderColor: 'rgb(18, 99, 32)',
            data: prediction.map(dustdata => dustdata.particulateMatter25),
          },
          {
            label: 'Dust 10.0',
            // backgroundColor: 'rgb(18, 3, 132)',
            borderColor: 'rgb(18, 3, 32)',
            data: (prediction.map(dustdata => dustdata.particulateMatter100)),
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
