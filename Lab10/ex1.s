/*
struct X {
  int val;
  struct X *next;
};

int add (struct X *x) {
  int a = 0;
  for (; x != NULL; x = x->next)
    a += x->val;
  return a;
}
*/

.text
.globl add

add:
    pushq   %rbp    #salva a base do RA da função chamadora
    movq    %rsp, %rbp  # cria a base do RA da função chamada
#  int a = 0;
    movl    $0, %eax  # Zera o registrador %eax, que armazenará a soma 'a'

for:
# for (; x != NULL; x = x->next)
    cmpq    $0, %rdi
    je      end

# a += x->val;
    movl  (%rdi), %edx  #x->val
    addl  %edx, %eax    #a += x->val

# Incremento
    movq   4(%rdi), rdi
    jmp    for
    
end:
    leave # Restaura a pilha e o Base Pointer 
    ret   # Retorna para a função chamadora 

/*
Dicionário
x   rdi
a   eax
*/