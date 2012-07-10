#include <iostream>
#include "segment.h"

using namespace std;


int main()
{
  string text="这个端午节，带着二十岁出头社会新鲜人的满腔热血和迷茫，用省吃俭用的生活费买了机票从广州飞回上海。三个月没有回家，想念妈妈做的饭菜，更记得临走前的小约定：终于回国工作，不再像是过去五年在外国生活几年见一次，所以要一个月回家一次。现今尽管没做到，但至少，该回家了。";
  vector<SegRes> result;
  result.clear();
  
  CSegment::getInstance().segment(text,result); //do segment
  
  vector<SegRes>::iterator it = result.begin();
  for(;it != result.end(); ++it)
    cout<<it->word<<"\t\t"<<it->type<<"\t"<<it->times<<endl;
  return 0;
}
