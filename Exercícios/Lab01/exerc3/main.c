#include "stack.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

void calcula(Stack *operandos, int *num_1, int *num_2, char *op){
    int *result = malloc(sizeof(int));
    
    if (*op == '+'){
        *result = *num_1 + *num_2;
    }else if (*op == '-'){
        *result = *num_1 - *num_2;
    }else if (*op == '*'){
        *result = *num_1 * *num_2;
    }else if (*op == '/'){
        *result = *num_1 / *num_2;
    }

    stack_push(operandos, result);

    free(num_1);
    free(num_2);
    free(op);
}

int main(){
    Stack *operandos = stack_construct();
    Stack *operadores = stack_construct();

    char string[100];

    scanf("%99[^\n]", string);

    for(int i = 0; string[i] != '\0'; i++){
            
        if(isdigit(string[i])){
            //se for valor, ele vai puxar para a pilha de operandos
            int *num = malloc(sizeof(int));
            *num = string[i] - '0';
            stack_push(operandos, num);
        }else if(string[i] == '+' || string[i] == '-' || string[i] == '*' || string[i] == '/'){
            //se for operador, ele vai puxar para a pilha de operadores
            char *op = malloc(sizeof(char));
            *op = string[i];
            stack_push(operadores, op);
        }else if(string[i] == ')'){
            //se for um fecha parênteses, ele popa os dois últimos números e o último operando;
            int *num_1 = stack_pop(operandos);
            int *num_2 = stack_pop(operandos);
            char *operador = stack_pop(operadores);

            calcula(operandos, num_2, num_1, operador);
        }else{
            continue;
        }
    }

    int *result = stack_pop(operandos);
    printf("Resultado: %d\n", *result);
    free(result);

    stack_destroy(operandos);
    stack_destroy(operadores);

    return 0;
}