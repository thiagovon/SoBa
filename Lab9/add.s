#int add (int a, int b, int c) {
    .globl add
add:
    pushq   %rbp        # salva a base do RA da função chamadora
    movq    $rsp, %rbp  # cria a base do RA da função chamada  
    subq    $16, %rsp  # abre espaço para o RA da função chamada

    # salvar valor dos registradores callee-saved
    movq    %r13, -8(%rbp)  # salva o valor de  r13 na função chamadora

#  return a+b+c;
#   temp = c            c está em edx (3o parâmetro)
    movl    %edx, %r13d

#    temp = temp + b    b está em %esi (2o parâmetro)
    addl    %esi, %r13d

#    temp = temp + a    a está em %edi (1o parâmetro)
    addl    %edi, %r13d

#    return temp
    movl    %r13d, %eax

#}
    #restaurar valores dos registrados callee-saved
    movq    -8(%rbp), %r13