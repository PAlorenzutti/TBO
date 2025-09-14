#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "page.h"

struct PageNode {
    Page* current; // Val
    PageNode* next;
};

struct Links {
    int size; // Quantidade de links
    PageNode* first_node; // Ponteiro para o primeiro nó
    PageNode* last_node; // Ponteiro para o último nó 
};

struct Page {
    char* name;
    Page** pages_in; // Lista de ponteiros para as paginas chegando nela
    Links* l_in; // Testando somente
    double page_rank; 
    double prev_page_rank; 
    int n_page_in;
    int n_page_out; 
};

PageNode* page_node_create(Page* page) {
    PageNode* node = (PageNode*)malloc(sizeof(PageNode));
    if (node == NULL) return NULL;
    
    node->current = page;
    node->next = NULL;
    return node;
}

void page_node_destroy(PageNode* node) {
    if (node == NULL) return;
    free(node);
}

Links* links_create(){
    Links* l = (Links*)malloc(sizeof(Links));
    l->first_node = NULL, l->last_node = NULL; 
    l->size = 0;
    return l;
}

Page*  list_pop(Links *l){

    if (l == NULL || l->first_node == NULL) return NULL;

    PageNode* to_destroy = l->first_node;

    Page * to_return = to_destroy->current;

    l->first_node = l->first_node->next;

    page_node_destroy(to_destroy);
    
    l->size--;

    if (l->first_node == NULL) l->last_node = NULL;

    return to_return;
}

void links_clear(Links * l){
    if (l == NULL) return;
    
    while (l->first_node != NULL) {
        list_pop(l);
    }
}

void links_destroy(void * links){
    Links* l = (Links*)links;
    links_clear(l);
    free(l); 
}

void links_add_page(Links* links, Page* page){
    if (links == NULL || page == NULL) return;
    
    PageNode* new_node = page_node_create(page);
    if (new_node == NULL) return;
    
    if (links->size == 0) {
        links->first_node = new_node;
        links->last_node = new_node;
    } else {
        links->last_node->next = new_node;
        links->last_node = new_node;
    }
    links->size++;
}

// ##############################################################################################

Page * page_create(const char* name, int n){

    Page * page = (Page*)malloc(sizeof(Page));
    
    page->name = (char*)malloc(sizeof(char*) * strlen(name) + 1);

    page->pages_in = (Page**)malloc(sizeof(Page*) * n); 
    
    page->l_in = links_create(); 

    strcpy(page->name, name);

    page->n_page_in = 0;

    page->n_page_out = 0;

    page->prev_page_rank = 0;

    page->page_rank = ((double)1/(double)n); //Page Rank inicial!

    page->prev_page_rank = ((double)1/(double)n);

    return page; 
}

Page ** get_in(void * page){
    Page * p = (Page*)page; 
    return p->pages_in;
}

char * get_name_page(void * page){
    Page * p = (Page*)page; 
    return p->name;
}

double get_page_rank(void * page){
    Page * p = (Page*)page; 
    return p->page_rank;
}

double get_prev_page_rank(void * page){
    Page * p = (Page*)page;
    return p->prev_page_rank;
}

void set_page_rank(void * page, double new_page_rank){
    Page * p = (Page*)page; 
    p->prev_page_rank = p->page_rank;
    p->page_rank = new_page_rank;
}

int get_n_page_in(void * page){
    Page * p = (Page*)page; 
    return p->n_page_in;
}

int get_n_page_out(void * page){
    Page * p = (Page*)page; 
    return p->n_page_out;
}


void add_page_out(void *page, void * page_to_add){

    Page * p1 = (Page*)page;

    Page * p2 = (Page*)page_to_add;
    
    p1->n_page_out++;

    p2->pages_in[p2->n_page_in++] = p1; //Funcionando com o atual!

    links_add_page(p2->l_in, p1); 

}

void page_print(void * page){

    Page* p = (Page*) page;
    printf("\nA pagina %s tem %d pagina(s) apontando para ela\n", p->name, p->n_page_in);
    for (int i = 0; i < p->n_page_in; i ++) printf("%s -> %s \n", p->pages_in[i]->name, p->name);

    printf("A pagina %s aponta para %d pagina(s)\n", p->name, p->n_page_out);

    printf("%s tem PAGE RANK DE %.8f\n", p->name, p->page_rank);

    printf("O TESTE COM OS LINKS RESULTOU NISSO AQUI CHEGANDO NESSE NÓ: %d\n", p->l_in->size);
}

void page_destroy(void * page){

    Page* p = (Page*)page;
    
    free(p->pages_in);

    free(p->name);

    links_destroy(p->l_in);
        
    free(p);

}

int cmp_pages(const void* a,const void* b){

    Page* p1 = (Page*) a;
    
    Page* p2 = (Page*) b;

    if (p1->page_rank > p2->page_rank) return 1;
    
    else if (p1->page_rank == p2->page_rank){
        return strcmp(p1->name, p2->name); 
    }
    
    return -1;

}