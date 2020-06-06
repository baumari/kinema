#ifndef K_OPIONS_HH
#define K_OPIONS_HH

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
    std::string m_Description;
    bool m_flag;
    bool Find(const std::string&);

  public:
    _OptBase(std::string s, std::string l, std::string d) :
      m_Short(s),
      m_Long(l),
      m_Description(d),
      m_flag(false) {}
  };
  class _OptWOArg : public _OptBase{
  public:
    _OptWOArg(std::string s, std::string l, std::string d) :
      _OptBase(s, l, d) {}
    ~_OptWOArg() {}
  };
  class _OptWArg : public _OptBase{
  public:
    std::string m_val;
    std::string m_constraint;
    bool m_fString;
    bool m_fInt;
    bool m_fDouble;
  public:
    _OptWArg(std::string s, std::string l, std::string val, std::string d)
      : _OptBase(s, l, d),
	m_val(val),
	m_constraint(""),	
	m_fString(false),
	m_fInt(false),
	m_fDouble(false) {}
    ~_OptWArg() {}

  public:
    // return length of val type ex) string=6, int=3 etc..    
    std::size_t TypeLength();
    std::string Type(); // return "int" or "string" or "double"
  };

private:
  std::vector<_OptWOArg> m_OptListWithoutArg;
  std::vector<_OptWArg> m_OptListWithArg;
  int LeadArg; // index of the first argument except for optarg
  int nArg; // Number of arguments except for option 

private:
  bool IsLongOpt(char *argv);
  bool IsShortOpt(char *argv);
  bool IsOpt(char *argv);
  bool IsLong(const std::string& OptName);
  bool IsShort(const std::string& OptName);
  void CheckOptInput(const std::string& ShortOpt,
		     const std::string& LongOpt);
  std::vector<_OptWOArg>::iterator Find(std::vector<_OptWOArg>&,
					const std::string &);
  std::vector<_OptWArg>::iterator Find(std::vector<_OptWArg>&,
				       const std::string &);
  std::size_t GetMaxOptLength(std::vector<_OptWOArg>&); // obsolete
  std::size_t GetMaxOptLength(std::vector<_OptWArg>&); // obsolete
  std::size_t GetMaxOptLength();

public:
  KOptions() : LeadArg(1), nArg(0) {} 
  ~KOptions() {}

public:
  void Add(std::string ShortOpt, std::string LongOpt,
	   std::string Description);
  // OptVal is set as a default value
  void Add(std::string ShortOpt, std::string LongOpt, int OptVal,
	   std::string Description, int nVal = 1);
  void Add(std::string ShortOpt, std::string LongOpt, double OptVal,
	   std::string Description, int nVal = 1);
  void Add(std::string ShortOpt, std::string LongOpt,
	   std::string OptVal, std::string Description, int nVal = 1);
  std::string Get(std::string OptName);
  bool Check(int argc, char* argv[]);
  bool Exist(const std::string &OptName);
  inline int Lead() {return LeadArg;}
  void Description();
  // add constraint to arguments on each option
  void Constraint(std::string OptName, std::string Condition);
};

#endif


