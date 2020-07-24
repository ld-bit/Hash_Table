#include<iostream>
#include<vector>
#include<string>
using namespace std;
struct strToInt1
{
 size_t operator()(const string& str)
 {
   size_t res=0;
   for(size_t i=0;i<str.size();i++)
   {
       res=res*131+str[i];
   }
   return res;
 }
};
struct strToInt2
{
 size_t operator()(const string& str)
 {
   size_t res=0;
   for(size_t i=0;i<str.size();i++)
   {
       res=res*65535+str[i];
   }
   return res;
 }
};
struct strToInt3
{
 size_t operator()(const string& str)
 {
   size_t res=0;
   size_t magic=63689;
   for(size_t i=0;i<str.size();i++)
   {
    res=res%magic+str[i];
    magic*=378551;
   }
   return res;
 }
};
class BitMap
{
public:
  BitMap(size_t range)
  {
    _table.resize(range/32+1,0);
  }
  //查询:
  bool Test(int cnt)
  {
    int idx=cnt/32;
    int bitIdx=cnt%32;
    if(((1<<bitIdx)&(_table[idx])))
      return true;
    return false;
  }
  //存储:set
  void Set(int cnt)
  {
    int idx=cnt/32;
    int bitIdx=cnt%32;
    _table[idx]=(_table[idx] | (1<<bitIdx));
  }
  //删除:set
  void ReSet(int cnt)
  {
    int idx=cnt/32;
    int bitIdx=cnt%32;
    int rest=~(1<<bitIdx);
    _table[idx]&=rest;
  }
private:
    vector<int>_table;
};
template <class T,class HF1,class HF2,class HF3>
class BloomFilter
{
public:
    BloomFilter(int range)
     :_bit(range*5)
      ,_bitcount(range*5)
    {}
    void Set(const T& val)
    {
      HF1 hf1;
      HF2 hf2;
      HF3 hf3;
      size_t hashCode1=hf1(val);
      size_t hashCode2=hf2(val);
      size_t hashCode3=hf3(val);
      _bit.Set(hashCode1%_bitcount);
      _bit.Set(hashCode2%_bitcount);
      _bit.Set(hashCode3%_bitcount);
    }
    bool Test(const T& val)
    {
     HF1 hf1;
     HF2 hf2;
     HF3 hf3;
     size_t hashCode1=hf1(val);
     size_t hashCode2=hf2(val);
     size_t hashCode3=hf3(val);
     if(_bit.Test(hashCode1%_bitcount)==false)
       return false;
     if(_bit.Test(hashCode2%_bitcount)==false)
       return false;
     if(_bit.Test(hashCode3%_bitcount)==false)
       return false;
     return true;
    }
private:
    BitMap _bit;
    size_t _bitcount;
};
int main()
{
  BloomFilter<string,strToInt1,strToInt2,strToInt3>bloom(100000);
  bloom.Set("asdasdada");
  bloom.Set("luodong");
  bloom.Set("asda");
  bloom.Set("asdada");
  cout<<bloom.Test("luodong")<<bloom.Test("wangdingyang")<<endl;
}
