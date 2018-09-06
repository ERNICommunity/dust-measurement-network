import { Component, Input } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { DatePipe } from '@angular/common';
import { Chart } from 'chart.js';
import { DustService } from '../service/dust.service';
import { DustDto } from '../service/DustDto';
import { forkJoin } from 'rxjs';

@Component({
  selector: 'app-chart',
  templateUrl: './chart.component.html',
  styleUrls: ['./chart.component.css']
})
export class ChartComponent {
  @Input() set id(value: number) {
    this.updateDustData(value);
  }

  constructor(private dustService: DustService) {}

  private updateDustData(id: number) {
    const historyObservable = this.dustService.getDustHistory(id, new Date(2010, 1, 1), new Date());
    const predictionObservable = this.dustService.getDustPrediction(id);
    forkJoin(historyObservable, predictionObservable).subscribe(
      result => this.drawChart(result[0], result[1]),
      err => console.error(err)
    );
  }

  private drawChart(history: DustDto[], prediction: DustDto[]) {
    const pipe = new DatePipe('en-us');
    const ctx = document.getElementById('chartcanvas') as HTMLCanvasElement;
    const chart = new Chart(ctx.getContext('2d'), {
      type: 'line',
      data: {
        labels: history.map(dustdata => pipe.transform(dustdata.timestamp, 'medium'))
        .concat(prediction.map(dustdata => pipe.transform(dustdata.timestamp, 'medium'))),
        datasets: [
          {
            label: 'Dust 2.5',
            // backgroundColor: 'rgb(18, 99, 132)',
            borderColor: 'rgb(18, 99, 32)',
            data: (history.map(dustdata => dustdata.particulateMatter25))
            .concat(prediction.map(dustdata => dustdata.particulateMatter25)),
          },
          {
            label: 'Dust 10.0',
            // backgroundColor: 'rgb(18, 3, 132)',
            borderColor: 'rgb(18, 3, 32)',
            data: (history.map(dustdata => dustdata.particulateMatter100))
            .concat(prediction.map(dustdata => dustdata.particulateMatter100)),
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
              min: 0,
              max: 200
            }
          }],
        }
      }
    });
  }
}
