#include <stdio.h>

void hoge(int i, int j){
  printf("%d, %d\n", i, j);
}

int main(){
  int i = 5;
  hoge(i, (i =10, i));
}
