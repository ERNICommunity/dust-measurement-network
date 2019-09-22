import { Component, Input, OnInit } from '@angular/core';
import { DustService } from '../service/dust.service';
import {  IChartistData } from 'chartist';

@Component({
  selector: 'app-prediction-chart',
  templateUrl: './prediction-chart.component.html',
  styleUrls: ['./prediction-chart.component.css']
})
export class PredictionChartComponent implements OnInit {
  private _id: number;
  data: IChartistData;

  @Input() set id(value: number) {
    this._id = value;
  }

  constructor(private _dustService: DustService) { }

  ngOnInit(): void {
    this.updateDustData();
  }

  private updateDustData(): void {
    this._dustService.getDustPrediction(this._id).subscribe(
      results => this.data = {
        labels: results.map(x => new Date(x.timestamp)),
        series: [
          results.map(x => x.particulateMatter25),
          results.map(x => x.particulateMatter100)
        ]
      },
      err => console.error(err)
    );
  }
}
