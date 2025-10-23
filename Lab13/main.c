#include <stdio.h>
#include <stdlib.h>

typedef int (*funcp)(int);



unsigned char codigo[] = {
    0x55,               //pushq %rbp
    0x48, 0x89, 0xe5,   //movq %rsp, %rbp              
    0x89, 0xf8          //mov %edi, %eax
    0xff, 0xc0          //inc %eax
    0xc9                //leave
    0xc3                //ret
}

int main (void) {
    funcp f;
    int a;
    int res;
    res = foo(a);
    printf("foo(%d) = %d\n", a, res);

    f = foo;
    a = 321;
    res = f(a);
    printf("foo(%d) = %d\n", a, res);

  return 0;
}


