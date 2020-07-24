#include<iostream>
#include<vector>
#include<utility>
using namespace std;
template<class V>
struct HashNode
{
  V _val;
  HashNode<V>* _next;
  HashNode(const V& val=V())
    :_val(val)
     ,_next(nullptr)
  {}
};
struct StrToInt
{
  size_t operator ()(const string& str) 
  {
    size_t res=0;
    for(auto& a: str)
    {
      res=res*131+a;
    }
    return res;
  }
};
template<class K>
struct hashFun{
   size_t operator () (const K& key)
   {
     return key;
   }
};
template<class K,class V,class KOV,class HF>
class HashTable;
template<class K,class V,class KOV,class HF>
struct HashIterator
{
 typedef HashTable<K,V,KOV,HF> HT;
 typedef HashNode<V> Node;
 typedef HashIterator<K,V,KOV,HF> Self;
 Node* _node;
 HT* _ht;
 HashIterator(Node* node,HT* ht) 
   :_node(node)
    ,_ht(ht)
  {}
 V& operator *()
 {
   return _node->_val;
 }
 V* operator ->()
 {
   return &_node->_val;
 }
 bool operator == (const Self& p)
 {
   return _node==p._node;
 }
 bool operator !=(const Self& p)
 {
   return _node!=p._node;
 }
 Self& operator ++()
 {
   KOV kov;
   HF hf;
   Node* cur=_node->_next;
   if(cur)
   {
    _node=_node->_next;
   }
   else 
   {
    size_t idx=hf(kov(_node->_val)) % _ht->_table.size();
    ++idx;
    for(;idx<_ht->_table.size();idx++)
    {
     if(_ht->_table[idx])
     {
       _node=_ht->_table[idx];
       break;
     }
    }
    if(idx==_ht->_table.size())
      _node=nullptr;
   }
   return *this; 
 }
};
template<class K,class V,class KOV,class HF>
class HashTable
{
public:
  friend struct HashIterator<K,V,KOV,HF>;
  typedef HashNode<V> Node;
  typedef HashIterator<K,V,KOV,HF> iterator;
  iterator begin()
  {
    for(int i=0;i<_table.size();i++)
    {
      Node* cur=_table[i];
      if(cur)
      {
        return iterator(cur,this);
      }
    }
     return iterator(nullptr,this);
  }
  iterator end()
  {
    return iterator(nullptr,this);
  }
  bool insert(const V& val)
  {
    //检查容量
    checkCapacity();
    
    //寻找插入位置
    KOV kov;
    HF hf;
    size_t idx=hf(kov(val)) % _table.size();
    Node* cur=_table[idx];
    while(cur)
    {
      if(kov(cur->_val)==kov(val))
      {
        return false;
      }
      cur=cur->_next;
    }
    //创建新的节点并插入
    cur=new Node(val);
    cur->_next=_table[idx];
    _table[idx]=cur;
    ++_size;
    return true;
  }
  void checkCapacity()
  {
    if(_size==_table.size())
    {
      //确定大小
      size_t newSize=_size==0?5:_size*2;
     //建立新表 
     vector<Node*> ht;
     ht.resize(newSize);
     //将旧表插入新表
     //1.拿到旧表的每一个位置 
     KOV kov;
     HF hf;
     for(size_t i=0;i<_size;i++) 
     {
     Node* cur=_table[i];
     while(cur) 
     {
       Node* next=cur->_next;
       size_t idx=hf(kov(cur->_val))%newSize;
       cur->_next=ht[idx];
       ht[idx]=cur;
       cur=next;
     }
     _table[i]=nullptr;
     }
     swap(_table,ht);
  }
  }
  Node* find(const K& key)
  {
    if(_table.size()==0)
      return nullptr;
    HF hf;
    KOV kov;
    int idx=hf(key)%_table.size();
    Node* cur=_table[idx];
    while(cur)
    {
      if(kov(cur->_val)==key)
      {
          return cur;
      }
      cur=cur->_next;
    }
    return nullptr;
  }
  bool erase(const K& key)
  {
    HF hf; 
    size_t idx=hf(key)%_size;
    Node* cur=_table[idx];
    Node* prev=cur;
    KOV kov;
    while(cur)
    {
      if(kov(cur->_val)==key)
      {
        if(prev==nullptr)
        {
          _table[idx]=cur->_next;
        }
        else 
        {
          prev->_next=cur->_next;
        }
        delete cur;
        return true;
      }
      prev=cur;
      cur=cur->_next;
    }
    return false;
  }
private:
  size_t _size=0;
  vector<Node*>_table;
};
template <class K,class V,class HF=hashFun<K>>
class Unorderedmap
{
struct KeyOfVal
{
 K operator ()(const pair<K,V>& value)  
 {
   return value.first;
 }
};
public:
  typedef typename HashTable<K,pair<K,V>,KeyOfVal,HF>::iterator iterator;
  iterator begin()
{
  return _table.begin();
}
 iterator end()
{
  return _table.end();
}
  bool insert(const pair<K,V>& val)
 {
  return _table.insert(val);
 }
HashNode<V>* find(const K& key)
{
  HashNode<V>* res=_table.find(key);
  if(res==nullptr)
  {
    return nullptr;
  }
  else 
  {
    return res;
  }
}
bool erase(const K& key)
{
   return _table.erase(key);
}
private:  
  HashTable<K,pair<K,V>,KeyOfVal,HF>_table;
};
void test()
{
  Unorderedmap<string,int,StrToInt> mp;
  mp.insert(make_pair("luodong",1));
  mp.insert(make_pair("wangdingyang",1));
  mp.insert(make_pair("xxx",1));
  mp.insert(make_pair("gdsadasd",1));
  mp.insert(make_pair("dasdsa",1));
  mp.insert(make_pair("dsadsadsa",1));
  Unorderedmap<string,int,StrToInt>::iterator p=mp.begin();
  while(p!=mp.end())
  {
    cout<<p->first<<"---->"<<p->second<<endl;
    ++p;
  }
}
int main()
{
  test();
  return 0;
}
