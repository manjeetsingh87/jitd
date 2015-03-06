#include <iostream>
#include "cog.hpp"
#include "rewrite.hpp"

using namespace std;

Iterator BTreeCog::iterator()
{
  return Iterator(new SeqIterator(lhs->iterator(), sep, rhs->iterator()));
}
void BTreeCog::printDebug(int depth)
{
  prefix(depth);
  cout << "BTree[" << sep << "]" << endl;
  lhs->printDebug(depth+1);
  rhs->printDebug(depth+1);
}
//void BTreeCog::rewrite_children(Rewrite &rw, Key target) 
//{
//  cout << "RW: " << &rw << endl;
//  if(sep > target){ rw.apply(lhs); }
//  if(sep < target){ rw.apply(rhs); }
//  cout << "done" << endl;
//}