#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void b(void)
{
  write(1, " ", 1);
}

void a(void)
{
  malloc(1000);
  b();
}

void foo(void)
{
  a();
}



int main(void)
{
  int arr[10000];
    malloc(100);
        foo();
    malloc(10);
    
    return 0;
}
