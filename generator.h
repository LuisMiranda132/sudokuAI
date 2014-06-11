#include "coder.h"
#include <cstdlib>
#include <array>
#define SIZE 81
#define NUM 9

void fill9(std::vector<signed char>& n){
  for(int i=0;i<NUM;++i){
    n.push_back(i+1);
  }
}

std::array<char,SIZE> generateSingle(int slots, std::vector<signed char> pos){
  int filledSlots = 0;
  std::array<char,SIZE> res;
  std::vector<signed char> nums;
  fill9(nums);
  int toErase = rand() % nums.size();
  int curChar = nums(toErase);
  nums.erase(nums.begin() + toErase);

  for(int i=0;i<SIZE;++i){
    toErase = rand() % pos.size();
    int curPos = pos[toErase];
    pos.erase(pos.begin() + toErase);
    if(filledSlots != slots){
      res[curPos] = ('0' + curChar);
      ++filledSlots;
    }else{
      res[curPos] = '.';
    }
    
  }
  return res;
}
