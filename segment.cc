#include "segment.h"

pthread_mutex_t imutex = PTHREAD_MUTEX_INITIALIZER;

CSegment* CSegment::instance_ = NULL;

CSegment& CSegment::get_instance() {
  if (!instance_) {
    pthread_mutex_lock(&imutex);
    if (!instance_)
      instance_ = new CSegment();
    pthread_mutex_unlock(&imutex);
  }
  return *instance_;
}

CSegment::CSegment() {
  seg_result_ = (char*) malloc(1024 * 1024); // 1M TODO

  string stop_word_file = "stopwords.txt";
  if (init_stop(stop_word_file.c_str()) == -1)
    cerr << "init_stop failed!" << endl;

  if (!ICTCLAS_Init())
    cerr << "Init failed!" << endl;

  ICTCLAS_SetPOSmap(2);
}

CSegment::~CSegment() {
  free(seg_result_);
  delete stop_words_set_;
  ICTCLAS_Exit();
}

int CSegment::init_stop(const char* stop_word_file) {
  std::ifstream input(stop_word_file, std::ifstream::in);
  if (!input)
    return -1;
  stop_words_set_ = new std::set<std::string>;
  std::string line;
  while (getline(input, line)) {
    if (line.length() == 0)
      continue;
    stop_words_set_->insert(line);
  }
  input.close();
  return 0;
}

bool CSegment::is_stop_word(const std::string& word) {
  return (stop_words_set_->find(word) != stop_words_set_->end());
}

void CSegment::segment(const string& text, vector<SegRes>& result,
    eCodeType type, int stop_flag) {
  map<string, int> unique_words;
  unique_words.clear();
  unsigned int nPaLen = text.length();
  int nRstLen = ICTCLAS_ParagraphProcess(text.c_str(), nPaLen, seg_result_, type,
      1);

  char* pword = strtok(seg_result_, " ");
  while (pword) {
    char* pp = strrchr(pword, '/');
    if (pp == NULL) { //space
      ;
    } else {
      while (pword[0] == '\r' || pword[0] == '\n')
        ++pword;
      ++unique_words[pword];
    }
    pword = strtok(NULL, " ");
  }
  map<string, int>::iterator it = unique_words.begin();
  for (; it != unique_words.end(); ++it) {
    SegRes sr;
    string::size_type pos = it->first.rfind('/');
    sr.word = it->first.substr(0, pos);
    if (stop_flag && is_stop_word(sr.word))
      continue;
    sr.type = it->first.substr(pos + 1);
    if (sr.type[0] == 'w')
      continue;
    sr.times = it->second;
    result.push_back(sr);
  }
}
