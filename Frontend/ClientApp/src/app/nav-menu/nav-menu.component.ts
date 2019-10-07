import { Component } from '@angular/core';
import { faMapMarkedAlt, faInfoCircle } from '@fortawesome/free-solid-svg-icons';
import { faGithub } from '@fortawesome/free-brands-svg-icons';

@Component({
  selector: 'app-nav-menu',
  templateUrl: './nav-menu.component.html',
  styleUrls: ['./nav-menu.component.css']
})
export class NavMenuComponent {
  isCollapsed = true;
  mapIcon = faMapMarkedAlt;
  aboutIcon = faInfoCircle;
  githubIcon = faGithub;

  toggle() {
    this.isCollapsed = !this.isCollapsed;
  }

  collapse() {
    this.isCollapsed = true;
  }
}
