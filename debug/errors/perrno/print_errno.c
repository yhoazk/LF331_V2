#include <stdio.h>

int main()
{
  int j;
  for(j=0;j < sys_nerr;++j)
  {
    printf("%d:  %s\n", j, sys_errlist[j]);
  }
  return 0;
}
