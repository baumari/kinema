#ifndef _KOPIONS_HH
#define _KOPIONS_HH

#include <string>
#include <vector>

class KOptions{
private:
  enum _OptType {
    LONG,
    SHORT,
    
    OPT_TYPE_SIZE,
  };
  class _OptBase{
  public:
    std::string m_Short;
    std::string m_Long;
    bool m_flag;
    bool find(const std::string&);

    _OptBase(){}
    ~_OptBase(){}
    _OptBase(const _OptBase& x) :
      m_Short(x.m_Short),
      m_Long(x.m_Long),
      m_flag(x.m_flag){}
  };
  class _OptWOArg : public _OptBase{};
  class _OptWArg : public _OptBase{
  public:
    std::string m_val;
    _OptWArg(const _OptWArg& x) :
      m_Short(x.m_Short),
      m_Long(x.m_Long),
      m_flag(x.m_flag),
      m_val(x.m_val){}
  };
  std::vector<_OptWOArg> m_OptListWithoutArg;
  std::vector<_OptWArg> m_OptListWithArg;

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
  std::vector<_OptWOArg>::iterator Find(const std::vector<_OptWOArg>&,
					const std::string &OptName);
  std::vector<_OptWArg>::iterator Find(const std::vector<_OptWArg>&,
				       const std::string &OptName);  
//  bool Exist(std::string OptName);
  
};

#endif


