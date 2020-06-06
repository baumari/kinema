#include <KOptions.hh>
#include <stdexcept>
#include <cstdio>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <iostream>
#include <iomanip>

static const int BUFF_L = 256;

void KOptions::Add(std::string ShortOpt, std::string LongOpt,
		   std::string Description){
  CheckOptInput(ShortOpt, LongOpt);
  _OptWOArg m_OptWOArg(ShortOpt, LongOpt, Description);
  m_OptListWithoutArg.push_back(m_OptWOArg);
}

void KOptions::Add(std::string ShortOpt, std::string LongOpt,
		   int OptVal, std::string Description, int nVal){
  CheckOptInput(ShortOpt, LongOpt);
  std::stringstream ss;
  ss << OptVal;
  _OptWArg m_OptWArg(ShortOpt, LongOpt, ss.str(), Description);
  m_OptWArg.m_fInt = true;
  m_OptListWithArg.push_back(m_OptWArg);
}

void KOptions::Add(std::string ShortOpt, std::string LongOpt,
		   double OptVal, std::string Description, int nVal){
  CheckOptInput(ShortOpt, LongOpt);
  std::stringstream ss;
  ss << OptVal;  
  _OptWArg m_OptWArg(ShortOpt, LongOpt, ss.str(), Description);
  m_OptWArg.m_fDouble = true;  
  m_OptListWithArg.push_back(m_OptWArg);  
}

void KOptions::Add(std::string ShortOpt, std::string LongOpt,
		   std::string OptVal, std::string Description, int nVal){
  CheckOptInput(ShortOpt, LongOpt);
  std::stringstream ss;
  ss << OptVal;
  _OptWArg m_OptWArg(ShortOpt, LongOpt, ss.str(), Description);
  m_OptWArg.m_fString = true;    
  m_OptListWithArg.push_back(m_OptWArg);  
}

bool KOptions::IsLongOpt(char *arg){
  if(arg[0] == '-' && arg[1] == '-'){
    if(strlen(arg)-2 > 1) return true;
    else{
      char tmp[BUFF_L];
      strncpy(tmp, &arg[2], strlen(arg)-2);
      fprintf(stderr, "%s may not be long option..\n", tmp);
      std::exit(EXIT_FAILURE);        
    }
  }else return false;
}

bool KOptions::IsShortOpt(char *arg){
  if(arg[0] == '-' && arg[1] != '-'){
    if(strlen(arg)-1 == 1) return true;
    else{
      char tmp[BUFF_L];
      strncpy(tmp, &arg[1], strlen(arg)-1);
      fprintf(stderr, "%s may not be short option..\n", tmp);
      std::exit(EXIT_FAILURE);              
    }
  }else return false;
}

bool KOptions::IsOpt(char *arg){
  if(arg[0] == '-') return true;
  else return false;
}

bool KOptions::Check(int argc, char* argv[]){ 
  std::string opt;
  for(int iarg=1; iarg<argc; iarg++){
    if(!IsOpt(argv[iarg])){
      nArg++;
      continue;
    }
    if(IsLongOpt(argv[iarg])){
      opt = argv[iarg];
      opt = opt.substr(2);
      if(iarg <= LeadArg) LeadArg++;
    }else if(IsShortOpt(argv[iarg])){
      opt = argv[iarg];
      opt = opt.substr(1);
      LeadArg++;      
    }
    if(opt.size() == 0){
      fprintf(stderr, "null-option is improbable!!\n");
      return false;
    }else{
      if(Find(m_OptListWithArg, opt) != m_OptListWithArg.end()){
	std::vector<_OptWArg>::iterator it = Find(m_OptListWithArg, opt);	
	it->m_flag = true;
	if(iarg+1 < argc){
	  if(!IsOpt(argv[iarg+1])){
	    it->m_val = argv[iarg+1];
	   if(iarg <= LeadArg) LeadArg++;
	  }else{
	    fprintf(stderr, "%s should be an argument for %s!\n\n",
		    argv[iarg+1], argv[iarg]);
	    return false;
	  }
	}else{
	  fprintf(stderr, "No argument for %s!\n\n", argv[iarg]);
	  return false;
	}
      }else if(Find(m_OptListWithoutArg, opt) != m_OptListWithoutArg.end()){
	std::vector<_OptWOArg>::iterator it = Find(m_OptListWithoutArg, opt);
	it->m_flag = true;
      }else{
	fprintf(stderr, "No such option (%s)!!\n\n", argv[iarg]);
	return false;
      }
    }
  }
  return true;
}

bool KOptions::Exist(const std::string &OptName){
  if(OptName.size() == 0){
    fprintf(stderr, "Argument for KOptions::Exist(std::string) should not be null-string!!\n");
    std::exit(EXIT_FAILURE);
  }else if(Find(m_OptListWithoutArg, OptName) != m_OptListWithoutArg.end()){
    std::vector<_OptWOArg>::iterator it = Find(m_OptListWithoutArg, OptName);
    return it->m_flag;
  }else if(Find(m_OptListWithArg, OptName) != m_OptListWithArg.end()){
    std::vector<_OptWArg>::iterator it = Find(m_OptListWithArg, OptName);
    return it->m_flag;
  }else{
    fprintf(stderr, "No such option (%s)!!\n", OptName.c_str());
    std::exit(EXIT_FAILURE);	
  }
}

std::vector<KOptions::_OptWOArg>::iterator KOptions::Find(std::vector<_OptWOArg> &v,
							  const std::string &OptName){
  std::vector<_OptWOArg>::iterator it;
  std::vector<_OptWOArg>::iterator it_end = v.end();  
  for(it = v.begin(); it != it_end; ++it){
    if(it->Find(OptName)) return it;
  }
  return it_end;
}

std::vector<KOptions::_OptWArg>::iterator KOptions::Find(std::vector<_OptWArg> &v,
							 const std::string &OptName){
  std::vector<_OptWArg>::iterator it;
  std::vector<_OptWArg>::iterator it_end = v.end();  
  for(it = v.begin(); it != it_end; ++it){
    if(it->Find(OptName)) return it;
  }
  return it_end;
}

bool KOptions::_OptBase::Find(const std::string &OptName){
  if(m_Short != OptName && m_Long != OptName) return false;
  else return true;
}

bool KOptions::IsLong(const std::string &OptName){
  if(OptName == "") return true;
  else if(OptName.size() <= 1 ) return false;
  else return true;
}

bool KOptions::IsShort(const std::string &OptName){
  if(OptName == "") return true;
  else if(OptName.size() <= 1) return true;
  else return false;
}

void KOptions::CheckOptInput(const std::string& ShortOpt,
			     const std::string& LongOpt){
  if(ShortOpt == "" && LongOpt == ""){
    fprintf(stderr, "Either of long or short option should not be null!!\n");
    std::exit(EXIT_FAILURE);
  }
  else if(IsLong(LongOpt) && IsShort(ShortOpt));
  else{
    fprintf(stderr, "Given option may be not properly assigned..\n");
    fprintf(stderr, "%s should be long option and %s should be short one!!\n",
	    LongOpt.c_str(), ShortOpt.c_str());
    std::exit(EXIT_FAILURE);    
  }
}

std::string KOptions::Get(std::string OptName){
  if(OptName.size() == 0){
    fprintf(stderr, "Argument for KOptions::Get(std::string) should not be null-string!!\n");
    std::exit(EXIT_FAILURE);
  }else if(Find(m_OptListWithArg, OptName) != m_OptListWithArg.end()){
    std::vector<_OptWArg>::iterator it = Find(m_OptListWithArg, OptName);
    return it->m_val;
  }else{
    fprintf(stderr, "No such option (%s)!!\n", OptName.c_str());
    std::exit(EXIT_FAILURE);	
  }  
}

std::size_t KOptions::_OptWArg::TypeLength(){
  if(m_fInt) return 5; // "int" + "[]" = 6
  else if(m_fString) return 8;
  else if(m_fDouble) return 8;
  else return 0;
}

std::size_t KOptions::GetMaxOptLength(std::vector<_OptWOArg>& m_OptListWithoutArg){
  std::vector<_OptWOArg>::iterator it = m_OptListWithoutArg.begin();
  std::vector<_OptWOArg>::iterator it_end = m_OptListWithoutArg.end();
  std::size_t MaxLength = 0;
  std::size_t Eval;
  for(; it != it_end; ++it){
    Eval = it->m_Long.size();
    MaxLength = (MaxLength < Eval) ? Eval : MaxLength;
  }
  return MaxLength;
}

std::size_t KOptions::GetMaxOptLength(std::vector<_OptWArg>& m_OptListWithArg){
  std::vector<_OptWArg>::iterator it = m_OptListWithArg.begin();
  std::vector<_OptWArg>::iterator it_end = m_OptListWithArg.end();
  std::size_t MaxLength = 0;
  std::size_t Eval;
  for(; it != it_end; ++it){
    Eval = it->m_Long.size() + it->TypeLength();
    MaxLength = (MaxLength < Eval) ? Eval : MaxLength;
  }
  return MaxLength;
}

std::size_t KOptions::GetMaxOptLength(){
  std::size_t MaxLength = 0;
  std::size_t Eval;
  for(auto x : m_OptListWithoutArg){
    Eval = 0;
    if(x.m_Short.size() != 0 && x.m_Long.size() != 0){
      Eval += strlen("-") + x.m_Short.size() + strlen(", ")
	+ strlen("--") + x.m_Long.size() + strlen(": ");
    }else if(x.m_Short.size() == 0){
      Eval += strlen("--") + x.m_Long.size() + strlen(": ");
    }else if(x.m_Long.size() == 0){
      Eval += strlen("-") + x.m_Short.size() + strlen(": ");
    }
    MaxLength = (MaxLength < Eval) ? Eval : MaxLength;
  }
  for(auto x : m_OptListWithArg){
    Eval = 0;
    if(x.m_Short.size() != 0 && x.m_Long.size() != 0){
      Eval += strlen("-") + x.m_Short.size() + strlen(", ")
	+ strlen("--") + x.m_Long.size() + strlen(":   ") + x.TypeLength();
    }else if(x.m_Short.size() == 0){
      Eval += strlen("--") + x.m_Long.size() + strlen(":   ")
	+ x.TypeLength();
    }else if(x.m_Long.size() == 0){
      Eval += strlen("-") + x.m_Short.size() + strlen(":   ")
	+ x.TypeLength();      
    }
    MaxLength = (MaxLength < Eval) ? Eval : MaxLength;    
  }
  return MaxLength;
}

std::string KOptions::_OptWArg::Type(){
  if(m_fInt) return "[int]";
  else if(m_fString) return "[string]";
  else if(m_fDouble) return "[double]";
  else return "";
}

// Print descriptions for registered options
// Format -- Options..
//        -- -h, --helpout [type of argument]: Show Help
//        --     --hoge    [type of argument]: fugafuga
//        -- -p            [type of argument]: piyo
void KOptions::Description(){
  if(m_OptListWithoutArg.size() == 0 &&
     m_OptListWithArg.size() == 0){
    fprintf(stderr, "KOption::Description: No option registered!!!\n");
    std::exit(EXIT_FAILURE);
  }
  /* get maximum of (short+long) option length */
  std::size_t MaxOptLength = GetMaxOptLength();

  /* print routine (without arguments)*/
  std::cout << "Options.." << std::endl;
  for(auto x : m_OptListWithoutArg){
    if(x.m_Short.size() == 0){
      std::string s = "--" + x.m_Long + ": ";
      std::cout << std::right << std::setw(MaxOptLength) << s;
      std::cout << x.m_Description << std::endl;
    }else if(x.m_Long.size() == 0){
      std::string s = "-" + x.m_Short + ": ";
      std::cout << std::right << std::setw(MaxOptLength) << s;
      std::cout << x.m_Description << std::endl;      
    }else{
      std::string s = "-" + x.m_Short + ", --" + x.m_Long + ": ";
      std::cout << std::right << std::setw(MaxOptLength) << s;
      std::cout << x.m_Description << std::endl;            
    }
  }
  for(auto x : m_OptListWithArg){
    if(x.m_Short.size() == 0){
      std::string s = "--" + x.m_Long + " " + x.Type() + " : ";
      std::cout << std::right << std::setw(MaxOptLength) << s;
      std::cout << x.m_Description << std::endl;      
    }else if(x.m_Long.size() == 0){
      std::string s = "-" + x.m_Short + " " + x.Type() + " : ";
      std::cout << std::right << std::setw(MaxOptLength) << s;
      std::cout << x.m_Description << std::endl;      
    }else{
      std::string s = "-" + x.m_Short + ", --" + x.m_Long + " " + x.Type() + " : ";
      std::cout << std::right << std::setw(MaxOptLength) << s;
      std::cout << x.m_Description << std::endl;            
    }
  }  
  return ;
}

void KOptions::Constraint(std::string OptName, std::string sConstraint){
  if(Find(m_OptListWithArg, OptName) == m_OptListWithArg.end()){
    fprintf(stderr, "No such option (%s)!!\n", OptName.c_str());
    std::exit(EXIT_FAILURE);
  }
  std::vector<_OptWArg>::iterator it = Find(m_OptListWithArg, OptName);
  it->m_constraint = sConstraint;
  
  return ;
}
