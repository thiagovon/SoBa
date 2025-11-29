/* 
  Felipe Bermudes de Oliveira Leite - 2421786 - 3WA
  Thiago Von Randow Fernandes - 2420001 - 3WA
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gera_codigo.h"

// Quantidade máxima de funções suportadas
#define MAX 50

/*
RA

v0 -> -4(%rbp)
v1 -> -8(%rbp)
v2 -> -12(%rbp)
v3 -> -16(%rbp)
v4 -> -20(%rbp)
p0 -> -24(%rbp)   (parâmetro salvo porque EDI pode ser sobrescrito em calls)
*/

// Função auxiliar: imprime mensagem de erro e aborta o programa
static void error (const char *msg, int line) {
    fprintf(stderr, "erro %s na linha %d\n", msg, line);
    exit(EXIT_FAILURE);
}

// emite um único byte no vetor 'cod' e avança 'ppos'
static void emite_byte(unsigned char cod[], int *ppos, unsigned char b){
  cod[*ppos] = b;    // coloca o byte na posicao atual 
  (*ppos)++;         // avanca para a proxima posicao 
}

// escreve um inteiro de 4 bytes em little-endian no vetor 'cod'
static void emite_int(unsigned char cod[], int *ppos, int v){
  int *p = (int *)&cod[*ppos]; // interpreta posição corrente como int*
  *p = v;             // escreve 4 bytes equivalentes ao inteiro
  *ppos += 4;        // anda 4 posicoes 
}

// Gera o prólogo padrão da função:
// push %rbp
// mov %rsp, %rbp
// sub $32, %rsp   (aloca espaço para as 5 variáveis + p0)
// mov %edi, -24(%rbp) (salva p0 no RA)
static void gera_prologo(unsigned char cod[], int *ppos){
  emite_byte(cod, ppos, 0x55);  // push %rbp
  emite_byte(cod, ppos, 0x48);  // REX
  emite_byte(cod, ppos, 0x89);
  emite_byte(cod, ppos, 0xE5); // mov %rsp, %rbp

  // subq $32, %rsp  
  emite_byte(cod, ppos, 0x48);
  emite_byte(cod, ppos, 0x83);
  emite_byte(cod, ppos, 0xEC);
  emite_byte(cod, ppos, 0x20); // 32 bytes

  // mov %edi, -24(%rbp)   (EDI = p0)
  emite_byte(cod, ppos, 0x89);
  emite_byte(cod, ppos, 0x7D);
  emite_byte(cod, ppos, 0xE8);  // -24 = 0xE8
}

// epílogo padrão da função:
// leave
// ret
static void gera_epilogo(unsigned char cod[], int *ppos) {
  emite_byte(cod, ppos, 0xC9);  // leave
  emite_byte(cod, ppos, 0xC3);  // ret
}

// Carrega um varpc (variável, parâmetro ou constante) para %eax
static void carrega_varpc_em_eax(unsigned char cod[], int *ppos, char var, int idx){
   if (var == '$') {
        // mov $constante, %eax
        emite_byte(cod, ppos, 0xB8);   
        emite_int (cod, ppos, idx);     
    }
    else if (var == 'v') {
        // mov -offset(%rbp), %eax
        int offset = -4 * (idx + 1);   
        emite_byte(cod, ppos, 0x8B);
        emite_byte(cod, ppos, 0x45);
        emite_byte(cod, ppos, (unsigned char)offset);
    }
    else if (var == 'p') {
        // única possibilidade é p0 → -24(%rbp)
        emite_byte(cod, ppos, 0x8B);
        emite_byte(cod, ppos, 0x45);
        emite_byte(cod, ppos, 0xE8);   
    }
    else {
        error("operando invalido no carregamento em EAX", 0);
    }
}


// Salva %eax no destino (vX ou p0)
static void salva_eax_em_var(unsigned char cod[], int *ppos, char var, int idx){
  if (var == 'v') {
    int offset = -4 * (idx + 1);

     // mov %eax, -offset(%rbp)
    emite_byte(cod, ppos, 0x89); 
    emite_byte(cod, ppos, 0x45); 
    emite_byte(cod, ppos, (unsigned char)offset);
  } else if (var == 'p') {
    // sobrescreve p0
    emite_byte(cod, ppos, 0x89); 
    emite_byte(cod, ppos, 0x45); 
    emite_byte(cod, ppos, 0xE8);
  } else {
    error("destino invalido no salvamento de EAX", 0);
  }
}


// Gera operação aritmética: var0 = var1 op var2
static void gera_oper(unsigned char cod[], int *ppos,
                      char var0, int idx0,
                      char var1, int idx1,
                      char op, char var2, int idx2)
{

  // carrega var1 em %eax
  carrega_varpc_em_eax(cod, ppos, var1, idx1);

   // agora carrega var2 em %ecx
  if (var2 == 'v') {
      int offset = -4 * (idx2 + 1);
      emite_byte(cod, ppos, 0x8B); 
      emite_byte(cod, ppos, 0x4D); 
      emite_byte(cod, ppos, (unsigned char)offset); 
  } 
  else if (var2 == 'p') {
      emite_byte(cod, ppos, 0x8B); 
      emite_byte(cod, ppos, 0x4D); 
      emite_byte(cod, ppos, 0xE8); 
  } 
  else if (var2 == '$') {
      emite_byte(cod, ppos, 0xB9); 
      emite_int(cod, ppos, idx2);
  } 
  else {
      error("operando invalido no load em ecx", 0);
  }

  // aplica operação (eax = eax op ecx)
  if (op == '+'){
    emite_byte(cod, ppos, 0x01); 
    emite_byte(cod, ppos, 0xC8); // add %ecx, %eax
  }
  else if (op == '-'){
    emite_byte(cod, ppos, 0x29); 
    emite_byte(cod, ppos, 0xC8); // sub %ecx, %eax
  }
  else if (op == '*'){
    emite_byte(cod, ppos, 0x0F); 
    emite_byte(cod, ppos, 0xAF); 
    emite_byte(cod, ppos, 0xC1); // imul %ecx, %eax
  }
  else {
    error("operador desconhecido", 0);
  }

  // salva resultado em var0
  salva_eax_em_var(cod, ppos, var0, idx0);
}

// Gera ret varpc
static void gera_ret(unsigned char cod[], int *ppos, char var0, int idx0) {
    carrega_varpc_em_eax(cod, ppos, var0, idx0); // carrega valor de retorno
    gera_epilogo(cod, ppos);  // leave/ret
}

// Gera retorno condicional:
//
// zret x y
//
// if x == 0, retorna y
static void gera_zret(unsigned char cod[], int *ppos,
                      char var0, int idx0,
                      char var1, int idx1)
{

  // calcula quantos bytes serão pulados caso x != 0
  int set_size;
  if (var1 == 'v') set_size = 3;       // mov -disp(%rbp), %eax
  else if (var1 == 'p') set_size = 3;  // mov -24(%rbp), %eax
  else set_size = 5;                   // mov $imm32, %eax

  int epilogue_size = 2;  // leave + ret
  int skip = set_size + epilogue_size;

  // carrega var0 em eax
  carrega_varpc_em_eax(cod, ppos, var0, idx0);
  // cmp %eax, 0
  emite_byte(cod, ppos, 0x83); 
  emite_byte(cod, ppos, 0xF8); 
  emite_byte(cod, ppos, 0x00);

  // jne pula
  emite_byte(cod, ppos, 0x75); 
  emite_byte(cod, ppos, (unsigned char)skip);

  // parte executada caso eax == 0:
  carrega_varpc_em_eax(cod, ppos, var1, idx1);
  gera_epilogo(cod, ppos);
}

// Gera instrução vX = call N varpc
static void gera_call(unsigned char cod[], int *ppos,
                      char var0, int idx0,
                      int num_func,
                      char var1, int idx1,
                      long end_funcoes[])
{
  // prepara argumento em %edi
  if (var1 == 'v') {
    int offset = -4 * (idx1 + 1);
    emite_byte(cod, ppos, 0x8B); 
    emite_byte(cod, ppos, 0x7D); 
    emite_byte(cod, ppos, (unsigned char)offset);
  } 
  else if (var1 == 'p') {
    emite_byte(cod, ppos, 0x8B); 
    emite_byte(cod, ppos, 0x7D); 
    emite_byte(cod, ppos, 0xE8);
  } 
  else if (var1 == '$') {
    emite_byte(cod, ppos, 0xBF); 
    emite_int(cod, ppos, idx1);
  } 
  else {
    error("operando invalido no call", 0);
  }

  // call rel32
  emite_byte(cod, ppos, 0xE8);

  long endereco_funcao = end_funcoes[num_func];   
  long rip_next        = (long)(&cod[*ppos + 4]); // endereço após call

  int rel = (int)(endereco_funcao - rip_next);
  emite_int(cod, ppos, rel); // escreve deslocamento

  // salva retorno (%eax) em var0
  salva_eax_em_var(cod, ppos, var0, idx0);
}


// Função principal do gerador de código
void gera_codigo (FILE *f, unsigned char code[], funcp *entry) {
  int line = 1;
  int c;
  int pos = 0;                 // posição atual de escrita no vetor code[]
  long end_funcoes[MAX];       // guarda endereço inicial de cada função
  int func_atual = 0;          // índice da função atual
  int inicio_ultima_funcao = 0;
    
  // lê arquivo caractere a caractere
  while ((c = fgetc(f)) != EOF) {

    switch (c) {
        // Reconhece início de função: "function"
        case 'f': { 
            char c0;
            if (fscanf(f, "unction%c", &c0) != 1)
                error("comando invalido", line);
            // registra ponto onde nova função começa
            inicio_ultima_funcao = pos;
            end_funcoes[func_atual++] = (long)&code[pos];

            // insere prologo dessa função
            gera_prologo(code, &pos); 
            break;
        }

        // Reconhece "end"
        case 'e': { 
            fscanf(f, "nd");   // consome 'nd'
            gera_epilogo(code, &pos);
        break;
}

        // ret varpc
        case 'r': { 
            char var0;
            int idx0 = 0;

            // lê o caractere após "ret "
            if (fscanf(f, "et %c", &var0) != 1)
                error("comando invalido", line);

            if (var0 == '$') {
                // constante
                if (fscanf(f, "%d", &idx0) != 1)
                    error("constante invalida em ret", line);
            }
            else if (var0 == 'p') {
                // apenas p0 é permitido, sem número extra
                char zero;
                if (fscanf(f, "%c", &zero) != 1 || zero != '0')
                    error("esperado p0 em ret", line);
                idx0 = 0; 
            }
            else if (var0 == 'v') {
                // variável local precisa de número
                if (fscanf(f, "%d", &idx0) != 1)
                    error("indice de variavel invalido em ret", line);
            }
            else {
                error("operando invalido em ret", line);
            }

            gera_ret(code, &pos, var0, idx0);
            break;
        }

        // zret x y
        case 'z': {  
            int idx0, idx1;
            char var0, var1;

            if (fscanf(f, "ret %c%d %c%d", &var0, &idx0, &var1, &idx1) != 4)
                error("comando invalido", line);

            gera_zret(code, &pos, var0, idx0, var1, idx1); // gera o retorno condicional
            break;
        }

        // atribuição: vX = ...
        case 'v': {  
            int idx0;
            char var0 = c, c0;

            if (fscanf(f, "%d = %c", &idx0, &c0) != 2)
                error("comando invalido", line);

            if (c0 == 'c') { // call

                int fun, idx1;
                char var1;

                if (fscanf(f, "all %d %c%d\n", &fun, &var1, &idx1) != 3)
                    error("comando invalido", line);

                gera_call(code, &pos, var0, idx0, fun, var1, idx1, end_funcoes); // gera o call
            }

            else { // operação aritmética

                int idx1, idx2;
                char var1 = c0, var2, op;

                if (fscanf(f, "%d %c %c%d", &idx1, &op, &var2, &idx2) != 4)
                    error("comando invalido", line);

                gera_oper(code, &pos, var0, idx0, var1, idx1, op, var2, idx2); // gera a operacao
            }

            break;
        }

        case '\n':
        case ' ':
        case '\t':
            /* ignora brancos soltos (o fscanf(" ") no fim já ajuda) */
            break;

        default:
            error("comando desconhecido", line);
    }

    line++;
    fscanf(f, " ");  // consome espaços entre tokens
  }
  // Endereço da última função (chamada pelo main)
  *entry = (funcp)(code + inicio_ultima_funcao);
}
