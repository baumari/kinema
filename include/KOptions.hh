#ifndef _KOPIONS_HH
#define _KOPIONS_HH

#include <string>
#include <map>
#include <vector>

class KOptions{
private:
  std::vector<std::string> m_SOptListWithoutArg; //short options
  std::vector<std::string> m_LOptListWithoutArg; //long options 
//  std::map<std::string, KOptArg> m_SOptListWithArg;
//  std::map<std::string, KOptArg> m_LOptListWithArg;    

public:
  KOptions() {}
  ~KOptions() {}

public:
//Add("short opt","long opt",optarg default val,optarg constraint)  
//  void Add(std::string LongOpt, std::string ShortOpt);
//  template <typename T>
//  void Add(std::string LongOpt, std::string ShortOpt, T OptArg);
};


#endif


