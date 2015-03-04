#ifndef _COG_H_SHIELD
#define _COG_H_SHIELD

#include <memory>
//#include <atomic>
#include <iostream>

#include "data.hpp"
#include "iterator.hpp"

class Cog;
typedef std::shared_ptr<Cog> CogPtr;

class CogHandleBase {
  /* atomic< */ CogPtr /* > */ ref;
  
  public:
    CogHandleBase(CogPtr init) : ref(init) {}
  
    CogPtr get() { return ref /*.load()*/;  }
    void swap(CogPtr &nref) { ref /*.store( */ = nref /*)*/; }
    
    Iterator iterator();
    void printDebug();
    void printDebug(int depth);
};

typedef std::shared_ptr<CogHandleBase> CogHandle;

typedef enum {
  COG_CONCAT,
  COG_BTREE,
  COG_ARRAY,
  COG_SORTED_ARRAY
} CogType;

class Cog {
  
  public:
    Cog(CogType type): type(type) {}
  
    virtual Iterator iterator() { 
      std::cerr << "An Iterator is Unimplemented" << std::endl;
      exit(-1);
    }
    
    void printDebug() { printDebug(0); }
    void prefix(int depth){ while(depth > 0){ std::cout << "  "; depth--; } }
    virtual void printDebug(int depth) {
      prefix(depth);
      std::cout << "???" << std::endl;
    }
    
    CogType type;
};

#define MakeHandle(a) CogHandle(new CogHandleBase(shared_ptr<Cog>(a)))


///////////// Cog-Specific Class Headers /////////////

class ConcatCog : public Cog 
{
  public:
    ConcatCog (CogHandle &lhs, CogHandle &rhs) :
      Cog(COG_CONCAT), lhs(lhs), rhs(rhs) {}
  
    CogHandle getLHS(){ return lhs; }
    CogHandle getRHS(){ return rhs; }
    
    Iterator iterator();
    void printDebug(int depth);
    
  private:
    CogHandle lhs;
    CogHandle rhs;
};

// LHS < k <= RHS
class BTreeCog : public Cog
{
  public:
    BTreeCog (CogHandle &lhs, Key sep, CogHandle &rhs) : 
      Cog(COG_BTREE), lhs(lhs), sep(sep), rhs(rhs) {}
  
    Key getSep(){ return sep; }
    CogHandle getLHS(){ return lhs; }
    CogHandle getRHS(){ return rhs; }
    
    Iterator iterator();
    void printDebug(int depth);
    
  private:
    Key sep;
    CogHandle lhs;
    CogHandle rhs;
  
};

class ArrayCog : public Cog 
{
  public:
    ArrayCog(Buffer buffer, unsigned int start, unsigned int len) :
      Cog(COG_ARRAY), buffer(buffer), start(start), len(len) {}
  
    Buffer getBuffer(){ return buffer; }
    unsigned int getStart(){ return start; }
    unsigned int getLen(){ return len; }
    
    Iterator iterator();
    void printDebug(int depth);
    
  private:
    Buffer buffer;
    unsigned int start;
    unsigned int len;
};

class SortedArrayCog : public Cog 
{
  public:
    SortedArrayCog(Buffer buffer, unsigned int start, unsigned int len) :
      Cog(COG_SORTED_ARRAY), buffer(buffer), start(start), len(len) {}
  
    Buffer getBuffer(){ return buffer; }
    unsigned int getStart(){ return start; }
    unsigned int getLen(){ return len; }
    
    Iterator iterator();
    void printDebug(int depth);
    
  private:
    Buffer buffer;
    unsigned int start;
    unsigned int len;
};

#endif //_COG_H_SHIELD