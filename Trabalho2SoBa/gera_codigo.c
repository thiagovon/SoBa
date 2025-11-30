/* 
  Felipe Bermudes de Oliveira Leite - 2421786 - 3WA
  Thiago Von Randow Fernandes - 2420001 - 3WA
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gera_codigo.h"

#define MAX 50 //definindo o limite de 50 func maximas

/*
RA
v0 -> -4(%rbp)
v1 -> -8(%rbp)
v2 -> -12(%rbp)
v3 -> -16(%rbp)
v4 -> -20(%rbp)
p0 -> -24(%rbp)   (precaver para caso o edi seja sobrescrito)
*/

// Função auxiliar (F.A): imprime erro e encerra o programa

static void error (const char *mensagem, int linha) {
    fprintf(stderr, "erro %s na linha %d\n", mensagem, linha);
    exit(EXIT_FAILURE);
}

// coloca-se um único byte no vetor e passa a posição com o "prox" 

static void escreve_byte(unsigned char vet[], int *prox, unsigned char bt){
  vet[*prox] = bt;    // pega o byte para colocar na posição atual do vetor 
  (*prox)++;         // avança no vetor para a proxima posição
}

// poem no vetor "vet" um inteiro de 4 bytes (little-endian)

static void escreve_int(unsigned char vet[], int *prox, int i){
  int *p = (int *)&vet[*prox]; // lida com a posicao como int*
  *p = i;             // escreve 4 bytes 
  *prox += 4;        // anda 4 na posicao
}

// Configuração inicial do registro de ativação (prólogo):
// 1. Preserva o RBP antigo
// 2. Inicializa o novo RBP com o valor de RSP
// 3. Aloca 32 bytes na pilha para variáveis
// 4. Salva o parâmetro p0 na posição alocada
static void gera_prologo(unsigned char vet[], int *prox){
  escreve_byte(vet, prox, 0x55); // Salva o contexto do RBP anterior
  escreve_byte(vet, prox, 0x48); // Prefixo REX.W
  escreve_byte(vet, prox, 0x89);
  escreve_byte(vet, prox, 0xE5); // Define a nova base da pilha (RBP = RSP)

  // Expande a pilha (subtrai do RSP) 
  escreve_byte(vet, prox, 0x48);
  escreve_byte(vet, prox, 0x83);
  escreve_byte(vet, prox, 0xEC);
  escreve_byte(vet, prox, 0x20); // Quantidade de bytes reservados (32)

  // Armazena p0 (EDI) na memória local
  escreve_byte(vet, prox, 0x89);
  escreve_byte(vet, prox, 0x7D);
  escreve_byte(vet, prox, 0xE8);  // Deslocamento de -24 em relação ao RBP
}

// Desfaz o registro de ativação e retorna o controle ao chamador:
// O comando 'leave' restaura RSP e RBP, o 'ret' desempilha o endereço de retorno.

static void gera_epilogo(unsigned char vet[], int *prox) {
  escreve_byte(vet, prox, 0xC9);  // leave
  escreve_byte(vet, prox, 0xC3);  // ret
}

// Abstração para carregar operandos de diferentes tipos para o acumulador (EAX)

static void carrega_varpc_em_eax(unsigned char vet[], int *prox, char var, int ind){
   if (var == '$') {
        // Operação Imediata: O valor da constante é copiado diretamente
        escreve_byte(vet, prox, 0xB8);   
        escreve_int (vet, prox, ind);     
    }

    else if (var == 'v') {
        // Acesso à Memória (Variável Local):
        // Calcula o endereço efetivo baseado no índice (inteiros de 4 bytes)
        int offset = -4 * (ind + 1);   
        escreve_byte(vet, prox, 0x8B);
        escreve_byte(vet, prox, 0x45);
        escreve_byte(vet, prox, (unsigned char)offset);
    }

    else if (var == 'p') {
        // Recuperação de Parâmetro:
        // Busca p0 na posição onde foi preservado (-24 do RBP)
        escreve_byte(vet, prox, 0x8B);
        escreve_byte(vet, prox, 0x45);
        escreve_byte(vet, prox, 0xE8);   
    }

    else {
        error("identificador de operando inválido.", 0);
    }
}


// Transfere o conteúdo do registrador de retorno (EAX) para a memória (variável ou parâmetro)
static void salva_eax_em_var(unsigned char vet[], int *prox, char var, int ind){
    if (var == 'v') {
    // Localiza a variável na pilha (cada int ocupa 4 bytes negativos a partir de RBP)
    int offset = -4 * (ind + 1);

    // Operação de Store (MOV %eax, -offset(%rbp))
    escreve_byte(vet, prox, 0x89); 
    escreve_byte(vet, prox, 0x45); 
    escreve_byte(vet, prox, (unsigned char)offset);
  } 

    else if (var == 'p') {
    // Atualiza a cópia local do parâmetro p0 (endereço fixo -24)
    escreve_byte(vet, prox, 0x89); 
    escreve_byte(vet, prox, 0x45); 
    escreve_byte(vet, prox, 0xE8);
  }
    else {
    error("Destino de atribuição inválido", 0);
  }
}


// instruções para cálculo aritmético e atribuição
static void gera_oper(unsigned char vet[], int *prox,
                      char var0, int ind0,
                      char var1, int ind1,
                      char op, char var2, int ind2)
{

  // Move o primeiro operando para o EAX
  carrega_varpc_em_eax(vet, prox, var1, ind1);

  // Move o segundo operando para o Registrador ECX
  if (var2 == 'v') {
      // Acesso à memória
      int offset = -4 * (ind2 + 1);
      escreve_byte(vet, prox, 0x8B); // MOV para registrador
      escreve_byte(vet, prox, 0x4D); 
      escreve_byte(vet, prox, (unsigned char)offset); 
  } 
  else if (var2 == 'p') {
      // Acesso ao Parâmetro (Endereço fixo no frame)
      escreve_byte(vet, prox, 0x8B); 
      escreve_byte(vet, prox, 0x4D); 
      escreve_byte(vet, prox, 0xE8); 
  } 
  else if (var2 == '$') {
      escreve_byte(vet, prox, 0xB9); // MOV imediato para ECX
      escreve_int(vet, prox, ind2);
  } 
  else {
      error("Tipo de operando inválido para carga em ECX", 0);
  }

  // aplica operação "eax = eax op ecx" (Resultado fica em EAX)
  if (op == '+'){
    escreve_byte(vet, prox, 0x01); // ADD (r/m32 para r32)
    escreve_byte(vet, prox, 0xC8); // Soma ECX ao EAX
  }
  else if (op == '-'){
    escreve_byte(vet, prox, 0x29); // SUB
    escreve_byte(vet, prox, 0xC8); // Subtrai ECX de EAX
  }
  else if (op == '*'){
    escreve_byte(vet, prox, 0x0F); // Prefixo para instruções
    escreve_byte(vet, prox, 0xAF); // IMUL
    escreve_byte(vet, prox, 0xC1); // Multiplica EAX por ECX
  }
  else {
    error("Operador aritmético desconhecido", 0);
  }

  // Salva o conteúdo de EAX na variável de destino
  salva_eax_em_var(vet, prox, var0, ind0);
}

// define o resultado e encerra a função
static void gera_ret(unsigned char vet[], int *prox, char var0, int ind0) {
    // move o operando de retorno para o acumulador (%eax)
    carrega_varpc_em_eax(vet, prox, var0, ind0); 

    // Desfaz o stack frame e executa o salto de retorno 
    gera_epilogo(vet, prox);  
}

// Geração de Retorno Condicional (ZRET):
// Realiza o retorno antecipado se o primeiro operando for nulo.
// if (var0 == 0) return var1;
static void gera_zret(unsigned char vet[], int *prox,
                      char var0, int ind0,
                      char var1, int ind1)
{

  //Pré-cálculo do deslocamento 
  // Precisamos saber o tamanho da instrução 'mov' de retorno para configurar o pulo do JNE.
  // O tamanho varia conforme o tipo do operando (var1)
  int set_size;
  if (var1 == 'v') set_size = 3;       // MOV memória -> reg (3 bytes)
  else if (var1 == 'p') set_size = 3;  // MOV memória -> reg (3 bytes)
  else set_size = 5;                   // MOV imediato -> reg (5 bytes)

  int epilogue_size = 2;  // leave + ret
  int skip = set_size + epilogue_size; // total de bytes a pular caso a condição falhe

  // avaliação da "var0 == 0"
  // carrega o operando de teste em %eax
  carrega_varpc_em_eax(vet, prox, var0, ind0);

  // subtrai 0 de %eax
  escreve_byte(vet, prox, 0x83); 
  escreve_byte(vet, prox, 0xF8); 
  escreve_byte(vet, prox, 0x00); // cmp $0, %eax

  // jne - se ZF=0 (var0 != 0), salta sobre o bloco de retorno
  escreve_byte(vet, prox, 0x75); 
  escreve_byte(vet, prox, (unsigned char)skip);

  // essa parte só executa se var0 == 0
  carrega_varpc_em_eax(vet, prox, var1, ind1);
  gera_epilogo(vet, prox);
}

// Gera instrução vX = call N varpc
static void gera_call(unsigned char vet[], int *prox,
                      char var0, int ind0,
                      int num_func,
                      char var1, int ind1,
                      long end_funcoes[])
{
  // Move o parâmetro para o registrador %edi antes de transferir o controle
  if (var1 == 'v') {
    int offset = -4 * (ind1 + 1);
    escreve_byte(vet, prox, 0x8B); // MOV memória -> reg
    escreve_byte(vet, prox, 0x7D); 
    escreve_byte(vet, prox, (unsigned char)offset);
  } 
  else if (var1 == 'p') {
    escreve_byte(vet, prox, 0x8B); 
    escreve_byte(vet, prox, 0x7D); 
    escreve_byte(vet, prox, 0xE8);
  } 
  else if (var1 == '$') {
    escreve_byte(vet, prox, 0xBF); // MOV imediato -> %edi
    escreve_int(vet, prox, ind1);
  } 
  else {
    error("Tipo de operando inválido para argumento de CALL", 0);
  }

  // call Rel32 - 0xE8
  escreve_byte(vet, prox, 0xE8);

  // resolução de Endereço (Linkagem):
  // o CALL usa endereçamento relativo ao PC  
  // offset = endereço_alvo - endereço_próxima_instrução
  long endereco_alvo = end_funcoes[num_func];   

  // o ponteiro *prox aponta para o início do offset (4 bytes). 
  // logo, a próxima instrução começa em *prox + 4.
  long next_rip = (long)(&vet[*prox + 4]); // endereço após call

  int desloc = (int)(endereco_alvo - next_rip);
  escreve_int(vet, prox, desloc); // grava o deslocamento calculado

  // o resultado da função chamada está em %eax
  salva_eax_em_var(vet, prox, var0, ind0);
}


// Função principal do gerador de código
void gera_codigo (FILE *f, unsigned char code[], funcp *entry) {
  int linha = 1;
  int c;
  int pos = 0;              // onde estamos escrevendo no vetor de bytes
  long end_funcoes[MAX];    // guarda onde começa cada função (para os CALLs)
  int id_funcao = 0;        // contador: função 0, função 1, função 2...
  int end_main = 0;         // O endereço da última função lida
    
  // lê arquivo
  while ((c = fgetc(f)) != EOF) {

    switch (c) {
        case 'f': { 
            char c0;
            if (fscanf(f, "unction%c", &c0) != 1)
                error("comando invalido", linha);

            // registra p inicio da func
            end_main = pos;
            end_funcoes[id_funcao++] = (long)&code[pos];

            // insere prologo dessa função
            gera_prologo(code, &pos); 
            break;
        }

        // detecta o marcador de fim de escopo da função
        case 'e': { 
            fscanf(f, "nd");
            gera_epilogo(code, &pos); // instrução para limpeza da pilha
        break;
}

        // processamento do ret
        case 'r': { 
            char var0;
            int ind0 = 0;

            // identifica o tipo do operando de retorno
            if (fscanf(f, "et %c", &var0) != 1)
                error("comando 'ret' malformado", linha);

            if (var0 == '$') {
                // caso constante, extrai o valor literal

                if (fscanf(f, "%d", &ind0) != 1)
                    error("constante invalida em ret", linha);
            }
            else if (var0 == 'p') {
                // valida se é 'p0' (único permitido)

                char zero;
                if (fscanf(f, "%c", &zero) != 1 || zero != '0')
                    error("apenas 'p0' é permitido em 'ret'", linha);
                ind0 = 0; 
            }
            else if (var0 == 'v') {
                // caso variável local: Extrai o índice da variável (v0..v4)
                if (fscanf(f, "%d", &ind0) != 1)
                    error("indice de variavel invalido em ret", linha);
            }
            else {
                error("operando desconhecido em 'ret'", linha);
            }

            // move o valor para %eax e retorna
            gera_ret(code, &pos, var0, ind0);
            break;
        }

        // processamento da instrução de retorno condicional ("zret")
        // retorna se o primeiro operando for zero 
        case 'z': {  
            int ind0, ind1;
            char var0, var1;

            // extrai os dois op: Condição (var0) e valor de retorno (var1)
            if (fscanf(f, "ret %c%d %c%d", &var0, &ind0, &var1, &ind1) != 4)
                error("operandos inválidos para 'zret'", linha);

            // gera código para teste condicional e possível retorno
            gera_zret(code, &pos, var0, ind0, var1, ind1);
            break;
        }

        // reconhecimento de Atribuição (vX = ...)
        case 'v': {  
            int ind0;
            char var0 = c, c0; // var0 recebe 'v'

            if (fscanf(f, "%d = %c", &ind0, &c0) != 2)
                error("Erro de sintaxe na atribuição", linha);

            // verifica se o R-Value é uma chamada de função
            if (c0 == 'c') { // call

                int fun, ind1;
                char var1;

                // ID da função e parâmetro
                if (fscanf(f, "all %d %c%d\n", &fun, &var1, &ind1) != 3)
                    error("chamada de função malformada", linha);

                // emite sequência de chamada (Passagem de param + CALL)
                gera_call(code, &pos, var0, ind0, fun, var1, ind1, end_funcoes); // gera o call
            }

            else { // caso contrário, expressão aritmética binária

                int ind1, ind2;
                char var1 = c0, var2, op; // var1 já pegou o primeiro char do operando

                // extração do operador e do segundo operando
                if (fscanf(f, "%d %c %c%d", &ind1, &op, &var2, &ind2) != 4)
                    error("erro de sintaxe em expressão aritmética", linha);

                // (Add, Sub, Mul)
                gera_oper(code, &pos, var0, ind0, var1, ind1, op, var2, ind2); // gera a operacao
            }

            break;
        }

        case '\n':
        case ' ':
        case '\t':
            // ignora brancos soltos 
            break;

        default:
            error("Token desconhecido", linha);
    } // fim do switch

    linha++; // incrementa contador para relatório de erros
    fscanf(f, " ");  // consome espaços em branco residuais entre comandos
  }
  // definição do Entry Point do programa:
  // calcula o endereço de memória absoluto da última função processada
  // para que o loader/main possa executá-la.
  *entry = (funcp)(code + end_main);
}
