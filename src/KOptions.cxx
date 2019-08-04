#include <KOptions.hh>
#include <stdexcept>
#include <cstdio>
#include <algorithm>
#include <sstream>
#include <cstring>

static const int BUFF_L = 1024;

void KOptions::Add(std::string LongOpt, std::string ShortOpt){
  _OptWOArg m_OptWOArg;
  m_OptWOArg.m_Long = LongOpt;
  m_OptWOArg.m_Short = ShortOpt;
  m_OptWOArg.m_flag = false;
  m_OptListWithoutArg.push_back(m_OptWOArg);
}

void KOptions::Add(std::string LongOpt, std::string ShortOpt,
		   int OptVal){
  _OptWArg m_OptWArg;  
  std::stringstream ss;
  ss << OptVal;
  m_OptWArg.m_Long = LongOpt;
  m_OptWArg.m_Short = ShortOpt;
  m_OptWArg.m_val = ss.str();
  m_OptWArg.m_flag = false;
  m_OptListWithArg.push_back(m_OptWArg);  
}

void KOptions::Add(std::string LongOpt, std::string ShortOpt,
		   double OptVal){
  _OptWArg m_OptWArg;  
  std::stringstream ss;
  ss << OptVal;
  m_OptWArg.m_Long = LongOpt;
  m_OptWArg.m_Short = ShortOpt;
  m_OptWArg.m_val = ss.str();
  m_OptWArg.m_flag = false;
  m_OptListWithArg.push_back(m_OptWArg);  
}

void KOptions::Add(std::string LongOpt, std::string ShortOpt,
		   std::string OptVal){
  _OptWArg m_OptWArg;  
  std::stringstream ss;
  ss << OptVal;
  m_OptWArg.m_Long = LongOpt;
  m_OptWArg.m_Short = ShortOpt;
  m_OptWArg.m_val = ss.str();
  m_OptWArg.m_flag = false;
  m_OptListWithArg.push_back(m_OptWArg);  
}

//int KOptions::GetI(std::string OptName){
//  int val=-1;
//  try{
//    val=std::stoi(m_OptListWithArg[LONG].at(OptName));
//  }catch (std::out_of_range& oor){
//    try{
//      val=std::stoi(m_OptListWithArg[SHORT].at(OptName));
//    } catch (std::out_of_range& oor){
//      fprintf(stderr, "Out of Range: OptList\n");
//    }    
//  }
//  return val;
//}
//
//double KOptions::GetD(std::string OptName){
//  double val=-1;
//  try{
//    val=std::stod(m_OptListWithArg[LONG].at(OptName));
//  }catch (std::out_of_range& oor){
//    try{
//      val=std::stod(m_OptListWithArg[SHORT].at(OptName));
//    } catch (std::out_of_range& oor){
//      fprintf(stderr, "Out of Range: OptList\n");
//    }    
//  }
//  return val;
//}
//
//float KOptions::GetF(std::string OptName){
//  float val=-1;
//  try{
//    val=std::stof(m_OptListWithArg[LONG].at(OptName));
//  }catch (std::out_of_range& oor){
//    try{
//      val=std::stof(m_OptListWithArg[SHORT].at(OptName));
//    } catch (std::out_of_range& oor){
//      fprintf(stderr, "Out of Range: OptList\n");
//    }    
//  }
//  return val;
//}
//
//std::string KOptions::GetS(std::string OptName){
//  std::string val("Hey!! Error!!");
//  try{
//    val=m_OptListWithArg[LONG].at(OptName);
//  }catch (std::out_of_range& oor){
//    try{
//      val=m_OptListWithArg[SHORT].at(OptName);
//    } catch (std::out_of_range& oor){
//      fprintf(stderr, "Out of Range: OptList\n");
//    }    
//  }
//  return val;
//}
//

bool KOptions::IsLongOpt(char *arg){
  if(arg[0] == '-' && arg[1] == '-') return true;
  else return false;
}

bool KOptions::IsShortOpt(char *arg){
  if(arg[0] == '-' && arg[1] != '-') return true;
  else return false;
}

bool KOptions::IsOpt(char *arg){
  if(arg[0] == '-') return true;
  else return false;
}

// loop for enum is better to use..
//bool KOptions::Check(int argc, char* argv[]){ 
//  std::string opt;
//  for(int iarg=1; iarg<argc; iarg++){
//    if(IsLongOpt(argv[iarg])){
//      opt = argv[iarg];
//      opt = opt.substr(2);
//      if(Find(m_OptListWithoutArg, opt)
//	 != m_OptListWithoutArg.end()){
//	m_OptListWithoutArg = true;
//      }else if(m_OptListWithArg[LONG].find(opt)
//	       != m_OptListWithArg[LONG].end()){
//	m_OptListWithArg[LONG][opt].m_flag = true;
//	if(iarg+1 < argc){
//	  if(!IsOpt(argv[iarg+1])){
//	    m_OptListWithArg[LONG][opt].m_val = argv[iarg+1];
//	  }else{
//	    fprintf(stderr, "%s should be an argument for %s!\n\n",
//		    argv[iarg+1], argv[iarg]);
//	    return false;
//	  }
//	}else{
//	  fprintf(stderr, "No argument for %s!\n\n", argv[iarg]);
//	  return false;
//	}
//      }else{
//	fprintf(stderr, "No such option (%s)!!\n\n", argv[iarg]);
//	return false;
//      }
//    }else if(IsShortOpt(argv[iarg])){
//      opt = argv[iarg];
//      opt = opt.substr(1);
//      if(m_OptListWithoutArg[SHORT].find(opt)
//	 != m_OptListWithoutArg[SHORT].end()){
//	m_OptListWithoutArg[SHORT][opt] = true;
//      }else if(m_OptListWithArg[SHORT].find(opt)
//	       != m_OptListWithArg[SHORT].end()){
//	m_OptListWithArg[SHORT][opt].m_flag = true;
//	if(iarg+1 < argc){
//	  if(!IsOpt(argv[iarg+1])){
//	    m_OptListWithArg[SHORT][opt].m_val = argv[iarg+1];
//	  }else{
//	    fprintf(stderr, "%s should be an argument for %s!\n\n",
//		    argv[iarg+1], argv[iarg]);
//	    return false;	    
//	  }
//	}else{
//	  fprintf(stderr, "No argument for %s!\n\n", argv[iarg]);
//	  return false;
//	}
//      }else{
//	fprintf(stderr, "No such option (%s)!!\n\n", argv[iarg]);
//	return false;	
//      }
//    }
//  }
//  return true;
//}

// loop for enum is better to use..
bool KOptions::Check(int argc, char* argv[]){ 
  std::string opt;
  for(int iarg=1; iarg<argc; iarg++){
    if(IsLongOpt(argv[iarg])){
      opt = argv[iarg];
      opt = opt.substr(2);
    }else if(IsShortOpt(argv[iarg])){
      opt = argv[iarg];
      opt = opt.substr(1);      
    }
    if(opt.size() != 0){
      std::vector<_OptWOArg>::iterator it = Find(m_OptListWithoutArg, opt);
      if(it != m_OptListWithoutArg.end()){
	*it.m_flag = true;
	if(iarg+1 < argc){
	  if(!IsOpt(argv[iarg+1])){
	    *it.m_val = argv[iarg+1];
	  }else{
	    fprintf(stderr, "%s should be an argument for %s!\n\n",
		    argv[iarg+1], argv[iarg]);
	    return false;
	  }
	}else{
	  fprintf(stderr, "No argument for %s!\n\n", argv[iarg]);
	  return false;
	}
      }else{
	fprintf(stderr, "No such option (%s)!!\n\n", argv[iarg]);
	return false;
      }
    }
  }
  return true;
}


//// Exist must be called after KOptions::Check()
//bool KOptions::Exist(std::string OptName){
//  bool val;
//  if(OptName.size() == 0){
//    fprintf(stderr, "KOptions::Exist(std::string) must have non empty string!!\n");
//    fprintf(stderr, "Abort!\n");
//    std::exit(EXIT_FAILURE);    
//  }else if(OptName.size() == 1){
//    if(m_OptListWithoutArg[SHORT].find(OptName)
//       != m_OptListWithoutArg[SHORT].end()){
//      val = m_OptListWithoutArg[SHORT][OptName];
//    }else if(m_OptListWithArg[SHORT].find(OptName)
//	     != m_OptListWithArg[SHORT].end()){
//      val = m_OptListWithArg[SHORT][OptName].m_flag;
//    }else{
//      fprintf(stderr, "KOptions::Exist() must be called after KOptions::Check()!!\n");
//      fprintf(stderr, "Abort!\n");
//      std::exit(EXIT_FAILURE);      
//    }
//  }else{
//    if(m_OptListWithoutArg[LONG].find(OptName)
//       != m_OptListWithoutArg[LONG].end()){
//      val = m_OptListWithoutArg[LONG][OptName];
//    }else if(m_OptListWithArg[LONG].find(OptName)
//	     != m_OptListWithArg[LONG].end()){
//      val = m_OptListWithArg[LONG][OptName].m_flag;
//    }else{
//      fprintf(stderr, "KOptions::Exist() must be called after KOptions::Check()!!\n");
//      fprintf(stderr, "Abort!\n");
//      std::exit(EXIT_FAILURE);      
//    }
//  }
//  return val;
//}

std::vector<_OptWOArg>::iterator Find(const std::vector<_OptWOArg> &v,
				      const std::string &OptName){
  std::vector<_OptWOArg>::iterator it;
  std::vector<_OptWOArg>::iterator it_end = v.end();  
  for(it = v.begin(); it != it_end; ++it){
    if(*it.find(OptName)) return it;
  }
  return it_end;
}

std::vector<_OptWArg>::iterator Find(const std::vector<_OptWArg> &v,
				      const std::string &OptName){
  std::vector<_OptWArg>::iterator it;
  std::vector<_OptWArg>::iterator it_end = v.end();  
  for(it = v.begin(); it != it_end; ++it){
    if(*it.find(OptName)) return it;
  }
  return it_end;
}

bool KOptions::_OptBase::find(const std::string &OptName){
  if(m_Short != OptName && m_Long != OptName) return false;
  else return true;
}
