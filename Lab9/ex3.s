/*
void foo (int a[], int n) {
  int i;
  int s = 0;
  for (i=0; i<n; i++) {
    s += a[i];
    if (a[i] == 0) {
      a[i] = s;
      s = 0;
    }
  }
}
*/

.text
.globl foo

FOO:
# void foo (int a[], int n) {
    pushq   %rbp        # 1 - salva a base do RA 
    movq  %rsp, %rbp    # 2 - cria a base do RA da função chamada

# int i;
# int s = 0;

    movl    $0, %ecx
    




/*
Dicionário
int a[]     rdi 8 bytes
int n       esi 4 bytes
int i       edx 4 bytes
int s       ecx 4 bytes
*/