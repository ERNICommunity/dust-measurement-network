import { Component, OnInit } from '@angular/core';
import { Router, ActivatedRoute } from "@angular/router";
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
  idRange: number[] = [0,1,2,3,4,5,6,7,8,9];
  selectedId: number = 0;
  historyLoaded: boolean = false;
  predictionLoaded: boolean = false;

  constructor(private dustService: DustService, private activatedRoute: ActivatedRoute, private router: Router) {
    this.id = activatedRoute.snapshot.params['id'];
}

  ngOnInit() {
    this.updateDustData();
  }

  updateDustData() {
    this.dustService.getDustHistory(this.id, (dustData: DustData[]) =>{
      this.dustHistory = dustData;
      this.historyLoaded = true;
      if(this.predictionLoaded) {
        this.showChart();
      }
    })

    this.dustService.getDustPrediction(this.id, (dustData: DustData[]) =>{
      this.dustPrediction = dustData;
      this.predictionLoaded = true;
      if(this.historyLoaded) {
        this.showChart();
      }
    })
  }

  showChart() {
    this.ctx = document.getElementById('canvas');
    this.chart = new Chart(this.ctx.getContext('2d'), {
      type: 'line',
      data: {
        labels: (this.dustHistory.map(dustdata => dustdata.timestamp.toString())).concat(this.dustPrediction.map(dustdata => dustdata.timestamp.toString())),
        datasets: [
          {
            label: "Dust 2.5",
            //backgroundColor: 'rgb(18, 99, 132)',
            borderColor: 'rgb(18, 99, 32)',
            data: (this.dustHistory.map(dustdata => dustdata.dust1)).concat(this.dustPrediction.map(dustdata => dustdata.dust1)),
          },
          {
            label: "Dust 10.0",
            //backgroundColor: 'rgb(18, 3, 132)',
            borderColor: 'rgb(18, 3, 32)',
            data: (this.dustHistory.map(dustdata => dustdata.dust2)).concat(this.dustPrediction.map(dustdata => dustdata.dust2)),
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

  onIdSelected(){
    if (this.id != this.selectedId) {
      this.id = this.selectedId;
      this.updateDustData();
      this.router.navigate(['chart/' + this.selectedId]);
    }
   }
 

}
