#ifndef SEGMENT_H_
#define SEGMENT_H_

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <pthread.h>
#include <map>
#include <vector>
#include <set>
#include "ICTCLAS50.h"

using namespace std;

struct SegRes {
  string word;
  string type;
  unsigned int times;
};

class CSegment {

public:

  void segment(const string&, vector<SegRes>&, eCodeType type = CODE_TYPE_GB,
      int stop_flag = 1);
  static CSegment& get_instance();
  void release() { delete instance_; }
private:
  CSegment();
  ~CSegment();
  CSegment(const CSegment&);
  CSegment& operator=(const CSegment&);
  int init_stop(const char*);
  bool is_stop_word(const std::string&);

private:
  char *seg_result_; //buffer  
  static CSegment* instance_;
  std::set<std::string>* stop_words_set_;

};

#endif
