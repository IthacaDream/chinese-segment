#include <iostream>
#include "segment.h"

using namespace std;


int main()
{
  string text="�������ڣ����Ŷ�ʮ���ͷ��������˵���ǻ��Ѫ����ã����ʡ�Լ��õ���������˻�Ʊ�ӹ��ݷɻ��Ϻ���������û�лؼң������������ķ��ˣ����ǵ�����ǰ��СԼ�������ڻع��������������ǹ�ȥ���������������һ�Σ�����Ҫһ���»ؼ�һ�Ρ��ֽ񾡹�û�����������٣��ûؼ��ˡ�";
  vector<SegRes> result;
  result.clear();
  
  CSegment::getInstance().segment(text,result); //do segment
  
  vector<SegRes>::iterator it = result.begin();
  for(;it != result.end(); ++it)
    cout<<it->word<<"\t\t"<<it->type<<"\t"<<it->times<<endl;
  return 0;
}
