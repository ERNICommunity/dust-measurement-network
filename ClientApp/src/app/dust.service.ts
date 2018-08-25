import { Injectable } from '@angular/core';
import {HttpClient, HttpErrorResponse, HttpHeaders, HttpParams} from "@angular/common/http";

@Injectable()
export class DustService {

  constructor(private httpCLient: HttpClient) { }

  public searchBooks(keywords: string): Promise<Book[]> {
    let url = "/bookstore/api/books";
    let options = {
        params: new HttpParams().set('keywords', keywords)
    };
    return this.httpClient.get<Book[]>(url, options).toPromise()
        .then(books => {
            this.keywords = keywords;
            this.books = books;
            return books;
        })
        .catch((response: HttpErrorResponse) => {
            throw 'Unexpected error (HTTP status ' + response.status + ')';
        });
}



}
