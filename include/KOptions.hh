#ifndef _KOPIONS_HH
#define _KOPIONS_HH

#include <string>
#include <map>
#include <vector>

class KOptions{
private:
  enum OptType {
    LONG,
    SHORT,
    
    OPT_TYPE_SIZE,
  };
  std::vector<std::string>  m_OptListWithoutArg[OPT_TYPE_SIZE];
  std::map<std::string, std::string> m_OptListWithArg[OPT_TYPE_SIZE];
  
public:
  KOptions() {}
  ~KOptions() {}

public:
  void Add(std::string LongOpt, std::string ShortOpt);
  void Add(std::string LongOpt, std::string ShortOpt, int OptVal);
  void Add(std::string LongOpt, std::string ShortOpt, double OptVal);
  void Add(std::string LongOpt, std::string ShortOpt,
	   std::string OptVal);

  int GetI(std::string OptName);
  double GetD(std::string OptName);
  float GetF(std::string OptName);  
  std::string GetS(std::string OptName);    
};


#endif


