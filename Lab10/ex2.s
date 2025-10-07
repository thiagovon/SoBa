/*
int add2 (struct X *x) {
  if (x == NULL) return 0;
  else return x->val + add2(x->next);
}
*/

.text
.globl  add2

add2:
    pushq   %rbp    #salva a base do RA da função chamadora
    movq    %rsp, %rbp  # cria a base do RA da função chamada

# if (x == NULL) return 0;
    cmpq    $0, %rdi
    je      end

# return x->val + add2(x->next);
    movl    (%rdi), %edx  #x->val
    pushq   %rdx    


fim:
    leave # Restaura a pilha e o Base Pointer 
    ret   # Retorna para a função chamadora 



/*
Dicionário
x   rdi
a   eax
*/
