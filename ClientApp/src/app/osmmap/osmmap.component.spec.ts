import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { OsmMapComponent } from './osmmap.component';

describe('OsmMapComponent', () => {
  let component: OsmMapComponent;
  let fixture: ComponentFixture<OsmMapComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ OsmMapComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(OsmMapComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
