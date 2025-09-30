/*
int fat (int n) {
  if (n==0) return 1;
  else return n*fat(n-1);
}
/*
rip aponta para a próxima instrução a ser executada
rsp aponta para o topo da pilha
rbp aponta para a base do RA corrente
*/
/*
Dicionário
Reg     Vag
edi     n
*/

.text

#int fat (int n) {
.globl fat
fat:
    pushq   %rbp    #salva a base do RA da função chamadora
                    #realinha o topo da pilha em múltiplo de 16
    movq    %rsp, %rbp  # cria a base do RA da função chamada
    subq    $16, %rsp  #abre espaço para o RA da função chamada

    #TODO: salvar registradores callee-saved usados na função chamada 
    movq    %r15, -8(%rbp)  #salva o valor do r15 no RA

#  if (n==0) { 
    cmpl    $0, %edi
    jne     ELSE 

#    return 1;
    movl    $1, %eax
    jmp     RETORNA
#  }
#  else {
ELSE:
//    return n*fat(n-1);
#    temp = n
    movl    %edi, %r15d
#    temp = n-1;
    decl    %r15d

#    result = fat(temp);
    # salvar registradores caller-saved antes de chamar a função
    movl    %edi, -12(%rbp)

    # chamar a função
    # parâmetros
    movl    %r15d, %edi
    call    fat

    # restaurar os registradores caller-saved salvos
    movl    -12(%rbp), %edi

#    result = result * n;   em eax está o valor retornado
    imull   %edi, %eax

#    return result
    # mover o valor de result para eax
    # mas já está em eax
# }


RETORNA
    # restaurar os callee-saved
    movq    -8(%rbp), %r15
    # destruir o RA
    leave
# OU!!!!
#    movq    %rbp, %rsp
#    popq    %rbp

    # retornar
    ret