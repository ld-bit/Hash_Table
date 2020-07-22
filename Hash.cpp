#include<iostream>
#include<vector>
#include<utility>
using namespace std;
enum Status{
   EXIST,DEL,EMPTY
};
template<class K,class V>
struct HashNode
{
  pair<K,V>_val;
  Status _status;
  HashNode(const pair<K,V>& val=pair<K,V>())
    :_val(val)
     ,_status(EMPTY)
  {}
};
template<class K,class V>
class HashTable
{
public:
   HashTable(int n=10)
   {
    _table.resize(n);
    _size=0;
   }
   bool insert(const pair<K,V>& val)
   {
     checkCapacity();
     int idx= val.first%_table.size();
     while(_table[idx]._status==EXIST)
     {
       if(_table[idx]._val.first==val.first)
         return false;
       idx++;
       if(idx==_table.size())
         idx=0;
     }
     _table[idx]._val=val;
     _table[idx]._status=EXIST;
     _size++;
     return true;
   }
   void checkCapacity()
   {
     if((_size*10)/_table.size()>=7)
     {
         HashTable<K,V>ht(_table.size()*2);
         for(int i=0;i<_table.size();++i)
         {
           if(_table[i]._status==EXIST)
           {
              ht.insert(_table[i]._val);
           }
         }
         swap(ht._table,_table);
     }
   }
   HashNode<K,V>* find(const K& key)
   {
     int idx=key%_table.size();
     while(_table[idx]._status!=EMPTY)
     {
       if(_table[idx]._val.first==key&&_table[idx]._status==EXIST)
       {
         return &_table[idx];
       }
       idx++;
       if(idx==_table.size())
         idx=0;
     }
     return nullptr;
   }
   bool erase(const K& key)
   {
     HashNode<K,V>* ptr=find(key);
     if(ptr)
     {
      ptr->_status=DEL;
      --_size;
      return true;
     }
     return false;
   }
private:
  vector<HashNode<K,V>>_table;
  size_t _size;
};
int main()
{
  HashTable<int,int>hs;
  hs.insert(make_pair(1,1));
  hs.insert(make_pair(2,1));
  hs.insert(make_pair(3,1));
  hs.insert(make_pair(4,1));
  hs.insert(make_pair(6,1));
  hs.insert(make_pair(6,1));
  hs.insert(make_pair(7,1));
  hs.insert(make_pair(8,1));
  hs.insert(make_pair(9,1));
  hs.insert(make_pair(10,1));
  hs.insert(make_pair(11,1));
  hs.insert(make_pair(12,1));
  hs.insert(make_pair(13,1));
  cout<<hs.erase(18)<<" "<<hs.erase(12)<<endl;
  return 0;
}
