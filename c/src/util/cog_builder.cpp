
#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <stack>
#include <cstdlib>

#include "cog.hpp"
#include "cog_builder.hpp"

using namespace std;

CogHandle build_random_array(int len)
{
  int i;
  Buffer buff(new vector<Record>(len));
  for(i = 0; i < len; i++){
    (*buff)[i].key = rand() % 1000000;
    (*buff)[i].value = &buff + i;
  }
  return MakeHandle(new ArrayCog(buff, 0, len));
}


CogHandle build_cog(istream &input)
{
  shared_ptr<string> curr;
  stack<CogHandle> ret;
  string line;
  
  while(getline(input, line)){
    istringstream toks(line);
    string type;
    
    toks >> type;
    
    if(string("array") == type){
      string fill;
      toks >> fill;
      
      if(string("random") == fill) {
        int len;
        toks >> len;
        ret.push(build_random_array(len));        
      } else {
        cerr << "Invalid ArrayCog Fill Mode: " << fill << endl;
        exit(-1);
      }
      
    } else if(string("concat") == type) {
      CogHandle a = ret.top(); ret.pop();
      CogHandle b = ret.top(); ret.pop();
      
      ret.push(MakeHandle(new ConcatCog(a, b)));
    } else {
      cerr << "Invalid Cog Type: " << type << endl;
      exit(-1);
    }
  }
  
  CogHandle handle = ret.top();
  ret.pop();
  return handle;
}