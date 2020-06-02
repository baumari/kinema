#include <KOptions.hh>
#include <stdexcept>
#include <cstdio>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <iostream>

static const int BUFF_L = 256;

void KOptions::Add(std::string ShortOpt, std::string LongOpt,
		   std::string Description){
  CheckOptInput(LongOpt, ShortOpt);
  _OptWOArg m_OptWOArg(ShortOpt, LongOpt, Description);
  m_OptListWithoutArg.push_back(m_OptWOArg);
}

void KOptions::Add(std::string ShortOpt, std::string LongOpt,
		   int OptVal, std::string Description, int nVal){
  CheckOptInput(LongOpt, ShortOpt);
  std::stringstream ss;
  ss << OptVal;
  _OptWArg m_OptWArg(ShortOpt, LongOpt, ss.str(), Description);
  m_OptWArg.m_fInt = true;
  m_OptListWithArg.push_back(m_OptWArg);
}

void KOptions::Add(std::string ShortOpt, std::string LongOpt,
		   double OptVal, std::string Description, int nVal){
  CheckOptInput(LongOpt, ShortOpt);
  std::stringstream ss;
  ss << OptVal;  
  _OptWArg m_OptWArg(ShortOpt, LongOpt, ss.str(), Description);
  m_OptWArg.m_fDouble = true;  
  m_OptListWithArg.push_back(m_OptWArg);  
}

void KOptions::Add(std::string ShortOpt, std::string LongOpt,
		   std::string OptVal, std::string Description, int nVal){
  CheckOptInput(LongOpt, ShortOpt);
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
    if(!IsOpt(argv[iarg])) continue;
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
  if(OptName.size() <= 1) return false;
  else return true;
}

bool KOptions::IsShort(const std::string &OptName){
  if(OptName.size() <= 1) return true;
  else return false;
}

void KOptions::CheckOptInput(const std::string& LongOpt,
			     const std::string& ShortOpt){
  if(IsLong(LongOpt) && IsShort(ShortOpt));
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
    Eval = it->m_Long.size();
    MaxLength = (MaxLength < Eval) ? Eval : MaxLength;
  }
  return MaxLength;
}

// Print descriptions for registered options
// Format -- Options..
//        -- -h, --helpout: Show Help
//        --     --hoge   : fugafuga
//        -- -p           : piyo
void KOptions::Description(){
  if(m_OptListWithoutArg.size() == 0 &&
     m_OptListWithArg.size() == 0){
    fprintf(stderr, "KOption::Description: No option registered!!!\n");
    std::exit(EXIT_FAILURE);
  }
  /* get maximum option length */
  std::size_t MaxOptLengthWOArg = GetMaxOptLength(m_OptListWithoutArg);
  std::size_t MaxOptLengthWArg = GetMaxOptLength(m_OptListWithArg);
  std::size_t MaxOptLength
    = (MaxOptLengthWArg > MaxOptLengthWOArg) ? MaxOptLengthWArg : MaxOptLengthWOArg;
  std::string Blank;

  /* print loutine (without arguments)*/
  std::cout << "Options.." << std::endl;
  for(std::vector<_OptWOArg>::iterator it = m_OptListWithoutArg.begin();
      it != m_OptListWithoutArg.end(); ++it){
    if(it->m_Short.size() == 0) std::cout << "    ";
    else std::cout <<  "-" << it->m_Short << ", ";
    Blank.assign(MaxOptLength - it->m_Long.size(), ' ');
    if(it->m_Long.size() == 0) std::cout << "  " << Blank << ": ";
    else std::cout << "--" << it->m_Long << Blank << ": ";
    std::cout << it->m_Description << std::endl;
  }
  /* print loutine (with arguments)*/  
  for(std::vector<_OptWArg>::iterator it = m_OptListWithArg.begin();
      it != m_OptListWithArg.end(); ++it){
    if(it->m_Short.size() == 0) std::cout << "    ";
    else std::cout <<  "-" << it->m_Short << ", ";
    Blank.assign(MaxOptLength - it->m_Long.size(), ' ');
    if(it->m_Long.size() == 0) std::cout << "  " << Blank << ": ";
    else std::cout << "--" << it->m_Long << Blank << ": ";
    std::cout << it->m_Description << std::endl;
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
