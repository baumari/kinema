#ifndef _KOPIONS_HH
#define _KOPIONS_HH

#include <string>
#include <map>

class KOptions{
private:
  enum OptType {
    LONG,
    SHORT,
    
    OPT_TYPE_SIZE,
  };
  typedef struct {
    std::string m_val;
    bool m_flag;
    inline void pair(std::string val, bool flag){m_val=val;m_flag=flag;}
  } _OptPair;

  _OptPair m_OptPair[OPT_TYPE_SIZE];
  std::map<std::string, bool> m_OptListWithoutArg[OPT_TYPE_SIZE];
  std::map<std::string, _OptPair> m_OptListWithArg[OPT_TYPE_SIZE];

private:
  bool IsLongOpt(char *arg);
  bool IsShortOpt(char *arg);
  bool IsOpt(char *arg);    

public:
  KOptions() {} 
  ~KOptions() {}

public:
  void Add(std::string LongOpt, std::string ShortOpt);
  void Add(std::string LongOpt, std::string ShortOpt, int OptVal);
  void Add(std::string LongOpt, std::string ShortOpt, double OptVal);
  void Add(std::string LongOpt, std::string ShortOpt, std::string OptVal);

//  int GetI(std::string OptName);
//  double GetD(std::string OptName);
//  float GetF(std::string OptName);  
//  std::string GetS(std::string OptName);
//  template <typename T>
//  T Get(std::string OptName);
  bool Check(int argc, char* argv[]);
  //  bool Exist(std::string OptName);
};

#endif


