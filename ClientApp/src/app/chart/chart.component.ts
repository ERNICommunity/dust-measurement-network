import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from "@angular/router";
import { Chart } from 'chart.js';
import { DustService } from "../dust.service";
import { DustData } from "../definitions/DustData";

@Component({
  selector: 'app-chart',
  templateUrl: './chart.component.html',
  styleUrls: ['./chart.component.css']
})
export class ChartComponent implements OnInit {

  chart: Chart;
  ctx: any;
  dustHistory: DustData[] = [];
  dustPrediction: DustData[] = [];
  id: number = 0;

  constructor(private dustService: DustService, private activatedRoute: ActivatedRoute) {

    //let id = activatedRoute.snapshot.params['id'];
}

  ngOnInit() {

    this.dustService.getDustHistory(this.id, (dustData: DustData[]) =>{
      this.dustHistory = dustData;
      this.showChart();
    })

  }

  showChart() {
    this.ctx = document.getElementById('canvas');
    this.chart = new Chart(this.ctx.getContext('2d'), {
      type: 'line',
      data: {
        labels: this.dustHistory.map(dustdata => dustdata.timestamp.toString()),
        datasets: [
          {
            label: "Dust 2.5",
            //backgroundColor: 'rgb(18, 99, 132)',
            borderColor: 'rgb(18, 99, 32)',
            data: this.dustHistory.map(dustdata => dustdata.dust1),
          },
          {
            label: "Dust 10.0",
            //backgroundColor: 'rgb(18, 3, 132)',
            borderColor: 'rgb(18, 3, 32)',
            data: this.dustHistory.map(dustdata => dustdata.dust2),
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
