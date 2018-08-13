#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void b(void)
{
  write(1, " ", 1);
}

void cccc(void)
{
  write(1, " ", 1);
  malloc(0);
}

void a(void)
{
  cccc();
  malloc(1000);
  b();
}

void foo(void)
{
  a();
}



int main(void)
{
    malloc(100);
        foo();
    malloc(10); 
    return 0;
}
