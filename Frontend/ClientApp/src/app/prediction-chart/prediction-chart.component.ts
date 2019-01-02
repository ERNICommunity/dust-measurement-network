import { Component, Input, OnInit } from '@angular/core';
import { DustService } from '../service/dust.service';
import * as Chartist from 'chartist';

@Component({
  selector: 'app-prediction-chart',
  templateUrl: './prediction-chart.component.html',
  styleUrls: ['./prediction-chart.component.css']
})
export class PredictionChartComponent implements OnInit {
  private _id: number;

  @Input() set id(value: number) {
    this._id = value;
  }

  constructor(private _dustService: DustService) { }

  ngOnInit(): void {
    this.updateDustData();
  }

  private updateDustData(): void {
    this._dustService.getDustPrediction(this._id).subscribe(
      results => {
        const data =  { labels: [], series: [] };
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
