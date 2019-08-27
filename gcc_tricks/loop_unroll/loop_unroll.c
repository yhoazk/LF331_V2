#include <stdio.h>


void not_rolled(){
  int n = 10;
  for (; n > 0; n--){
    printf("kkkk\n");
  }

}


void rolled(){
  int k = 10;
  #pragma GCC ivdep
  for (; k > 0; k--){
    printf("----\n");
  }

}
int main(int argc, char const *argv[])
{
  not_rolled();
  rolled();
  return 0;
}
