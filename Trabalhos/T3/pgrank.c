#include "pgrank.h"
#include "page.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ALPHA 0.85
#define EPSILON 0.000001 //10^-6

void run_page_rank(void* p, int n){

    Page * page = (Page*) p;
    
    double prev_page_rank = get_page_rank(page);
    double calculo_somatorio = 0;
    
    double PR = (double)((1 - ALPHA)/n);
    
    if(get_n_page_out(page) == 0){
        PR += ALPHA * prev_page_rank;        
    }

    for(int i=0; i< get_n_page_in(page); i++){
        calculo_somatorio += (double)(get_prev_page_rank(get_in(page)[i]) / (double)(get_n_page_out(get_in(page)[i]))); 
    }

    PR += ALPHA * calculo_somatorio; 

    set_page_rank(page, PR); //deve setar o valor de prevPR com o PR atual e atualizar PR com o novo

}

int end_page_rank(void** pages, int n){

    double end = 0;
    
    for(int i=0; i<n; i++){
        end += fabs(get_page_rank(pages[i]) - get_prev_page_rank(pages[i])); //PREV PAGE_RANK?! 
    }

    end *= ((double)1/(double)n);
    if(end < EPSILON) return 1;
    
    return 0;
}

void power_method(void** pages, int n){

    int page_index = 0;

    while(1){

        for (int j = 0; j < n; j++)run_page_rank(pages[j], n);
        
        if(end_page_rank(pages, n)) return;
    }
}

void sort_pages(void ** pages, int n){

    Page ** pgs = (Page**) pages;
    
    qsort(pgs, n, sizeof(Page*), cmp_pages); 

}
