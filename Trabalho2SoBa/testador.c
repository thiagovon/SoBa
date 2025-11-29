/*
  Arquivo de teste para o gerador de código LBS.
  Compile com: gcc -Wall -Wa,--execstack -o lbs_compiler main.c gera_codigo.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gera_codigo.h"

int main(int argc, char *argv[]) {
    FILE *fp;
    funcp funcLBS;
    // Alocando um buffer grande para garantir espaço para o código de máquina
    unsigned char code[1024]; 
    int res;

    // Verifica argumentos
    if (argc < 3) {
        printf("Uso: %s <arquivo_lbs> <parametro_entrada>\n", argv[0]);
        printf("Exemplo: %s teste.lbs 5\n", argv[0]);
        return 1;
    }

    /* Abre o arquivo para leitura */
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo: %s\n", argv[1]);
        return 1;
    }

    // Limpa o buffer de código (opcional, ajuda na depuração visual)
    memset(code, 0, sizeof(code));

    /* Gera o codigo */
    // A função gera_codigo preenche o vetor 'code' e aponta 'funcLBS' 
    // para o endereço da última função encontrada.
    gera_codigo(fp, code, &funcLBS);
    
    fclose(fp);

    if (funcLBS == NULL) {
        printf("Erro na geracao do codigo (ponteiro de funcao nulo).\n");
        return 1;
    }

    int parametro = atoi(argv[2]);

    printf("--- Iniciando Execucao ---\n");
    printf("Endereco da funcao gerada: %p\n", funcLBS);
    printf("Parametro p0: %d\n", parametro);

    /* Chama a função gerada */
    // Aqui a mágica acontece: o processador salta para o vetor 'code'
    res = (*funcLBS)(parametro);

    printf("--- Execucao Finalizada ---\n");
    printf("Retorno da funcao LBS: %d\n", res);

    return 0;
}