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
    bool Find(const std::string&);

    _OptBase(){}
    ~_OptBase(){}
  };
  class _OptWOArg : public _OptBase{};
  class _OptWArg : public _OptBase{
  public:
    std::string m_val;
  };

private:
  std::vector<_OptWOArg> m_OptListWithoutArg;
  std::vector<_OptWArg> m_OptListWithArg;

private:
  bool IsLongOpt(char *argv);
  bool IsShortOpt(char *argv);
  bool IsOpt(char *argv);
  bool IsLong(const std::string& OptName);
  bool IsShort(const std::string& OptName);
  void CheckOptInput(const std::string& LongOpt,
		     const std::string& ShortOpt);
  std::vector<_OptWOArg>::iterator Find(std::vector<_OptWOArg>&,
					const std::string &);
  std::vector<_OptWArg>::iterator Find(std::vector<_OptWArg>&,
					const std::string &);
  
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
  bool Exist(const std::string &OptName);
};

#endif


