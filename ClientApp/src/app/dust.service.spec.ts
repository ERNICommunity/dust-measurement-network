import { TestBed, inject } from '@angular/core/testing';

import { DustService } from './dust.service';

describe('DustService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [DustService]
    });
  });

  it('should be created', inject([DustService], (service: DustService) => {
    expect(service).toBeTruthy();
  }));
});
