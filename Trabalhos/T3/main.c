#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"
#include "pgrank.h"

#include "rbt.h"
#include "str.h"
#include "vector.h"
#include "arquivo.h"

int main(int argc, char *argv[]) {
    // Inicia o programa com o caminho relativo passado;
    Program *p = program_init(argv[1]);

    // Realiza a leitura do arquivo index.txt, pegando o nome de todos as páginas utilizadas;
    index_read(p);

    // Realiza a leitura do arquivo stopwords.txt, pegando as todas as palavras que devem ser ignoradas;
    stopwords_read(p);

    // Realiza a leitura das páginas, pegando todos os termos e colocando na árvore de índices;
    pages_read(p);

    // Finaliza o programa
    program_destroy(p);
    Page ** pages = (Page**)malloc(sizeof(Page*) * 5); 

    Page * page0 = page_create("Pagina 0", 5);
    Page * page1 = page_create("Pagina 1", 5);
    Page * page2 = page_create("Pagina 2", 5); 
    Page * page3 = page_create("Pagina 3", 5);
    Page * page4 = page_create("Pagina 4", 5);

    add_page_out(page0, page3); //Refazendo o exemplo do professor 
    add_page_out(page0, page4);
    add_page_out(page0, page1); 

    add_page_out(page1, page2); 

    add_page_out(page3, page4);
    add_page_out(page3, page1);

    add_page_out(page4, page3);
    add_page_out(page4, page1);
    
    pages[0] = page0;
    pages[1] = page1;
    pages[2] = page2;
    pages[3] = page3;
    pages[4] = page4;
    

    power_method(pages, 5); 

    // page_print(page0);

    // page_print(page1);

    // page_print(page2);

    // page_print(page3);

    // page_print(page4);

    page_destroy(page0);
    page_destroy(page1);
    page_destroy(page2); 
    page_destroy(page3);
    page_destroy(page4);

    free(pages); 

    // // key: int *
    // // value: char *

    // FILE *f = fopen("in.txt", "r");
    // BinaryTree *bt = bt_init(compare_func, free_key_val, free_key_val);

    // int key = 0;
    // char value[40];

    // while (fscanf(f, "%d %s", &key, value) != -1) {
        
    //     bt_put(bt, generate_int(key), strdup(value));
    // }
    
    // bt_print(bt);
    // bt_destroy(bt);

    // fclose(f);

    // return 0;
}