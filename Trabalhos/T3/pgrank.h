#ifndef PG_RANK_H_
#define PG_RANK_H_

void run_page_rank(void* page, int n);

int end_page_rank(void** pages, int n);

void power_method(void** pages, int n);

void sort_pages(void ** pages, int n); 

#endif