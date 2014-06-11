#include "coder.h"
#include <unistd.h>
#include <fstream>
#include <sys/types.h>

int main(){
  std::string in;
  int caseno = 1;
  while(std::getline(std::cin,in)){
    std::string coded(code(in));
    std::ofstream res("code.txt");
    res << coded;
    res.close();
    
    system("minisat -verb=0 code.txt sat.txt > minisults");
    std::ifstream res2("sat.txt");
    std::string sat((std::istreambuf_iterator<char>(res2)), std::istreambuf_iterator<char>());
    res2.close();
    sat = sat.substr(3,std::string::npos);
    printf("Case %d ",caseno);
    ++caseno;
    decode(sat);
    system("rm code.txt sat.txt minisults");
  }
}
