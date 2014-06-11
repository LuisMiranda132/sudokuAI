#include "generator.h"
#include <time.h>

int main(){
  srand(time(NULL));
  std::vector<signed char> pos;
  for(int i=0;i<SIZE;++i){
    pos.push_back(i);
  }
  std::array<char,SIZE> test(generateSingle(80,pos));
  for(int i=0;i<test.size();++i){
    printf("%c",test[i]);
  }
  printf("\n");
}
