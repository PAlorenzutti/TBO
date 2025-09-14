#ifndef PAGE_H_
#define PAGE_H_

typedef struct PageNode PageNode;

typedef struct Links Links; 

typedef struct Page Page;

Page * page_create(const char* name, int n);

Page ** get_in(void * page); 

char * get_name_page(void * page);

double get_page_rank(void * page);

double get_prev_page_rank(void * page); 

void set_page_rank(void * page, double new_page_rank); 

int get_n_page_in(void * page);

int get_n_page_out(void * page); 

//PAGE TO ADD VAI APONTAR FAZER REFERENCIA NO GRAFO PARA PAGE, E PAGE VAI SER REFERENCIADA POR PAGE_TO_ADD

void add_page_in(void* page, void * page_to_add);

//PAGE TO ADD VAI SER REFERENCIADA NO GRAFO PARA PAGE_TO_ADD, E PAGE VAI SER REFERENCIAR PAGE_TO_ADD

void add_page_out(void *page, void * page_to_add); 

//Funcao de debug, pode ignorar
void page_print(void * page);

void page_destroy(void * page);

int cmp_pages(const void* a,const void* b);

#endif