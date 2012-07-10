#include "segment.h"

pthread_mutex_t imutex=PTHREAD_MUTEX_INITIALIZER;

CSegment* CSegment::instance = NULL;

CSegment& CSegment::getInstance()
{
  if(!instance) {
    pthread_mutex_lock(&imutex);
    if(!instance)
      instance=new CSegment();
    pthread_mutex_unlock(&imutex);
  }
  return *instance;
}

CSegment::CSegment()
{
  pResult = (char*)malloc(1024*1024);// 1M TODO
  
  string stop_word_file = "stopwords.txt";
  if(stopInit(stop_word_file.c_str()) == -1)
    cerr<<"stopInit failed!"<<endl;
  
  if(!ICTCLAS_Init())
    cerr<<"Init failed!"<<endl;
  
  ICTCLAS_SetPOSmap(2);
}

CSegment::~CSegment()
{
  free(pResult);
  delete stop_words_set;
  ICTCLAS_Exit();
}

int CSegment::stopInit(const char* stop_word_file)
{
  std::ifstream input(stop_word_file,std::ifstream::in);
  if(!input)
    return -1;
  stop_words_set = new std::set<std::string> ;
  std::string line;
  while (getline(input, line)) {
    if (line.length() == 0)
      continue;
    stop_words_set->insert(line);
  }
  input.close();
  return 0;
}

bool CSegment::isStopWord(const std::string& word) {
  return (stop_words_set->find(word) != stop_words_set->end());
}

void CSegment::segment(const string& text, vector<SegRes>& result, eCodeType type, int  stop_flag)
{
  map<string,int> unique_words;
  unique_words.clear();
  unsigned int nPaLen=text.length();
  int nRstLen = ICTCLAS_ParagraphProcess(text.c_str(),nPaLen,pResult,type,1);
  
  char* pword = strtok(pResult," ");
  while(pword){ 
    char* pp = strrchr(pword,'/');
    if(pp == NULL) { //space
      ;
    }
    else {
      //pp[0]='\0';
      while(pword[0]=='\r' || pword[0]=='\n')
	++pword;
      ++unique_words[pword];
    }
    pword = strtok(NULL," ");
  }
  map<string,int>::iterator it=unique_words.begin();
  for(;it != unique_words.end(); ++it){
    SegRes sr;
    string::size_type pos = it->first.rfind('/');
    sr.word = it->first.substr(0,pos);
    if( stop_flag && isStopWord(sr.word))
      continue;
    sr.type = it->first.substr(pos+1);
    if(sr.type[0] == 'w')
      continue;
    sr.times = it->second;
    result.push_back(sr);
  }
}
