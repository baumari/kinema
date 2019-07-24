#include <KOptions.hh>
#include <utility>
#include <stdexcept>
#include <cstdio>
#include <algorithm>
#include <sstream>
#include <cstring>

static const int BUFF_L = 1024;

void KOptions::Add(std::string LongOpt, std::string ShortOpt){
  m_OptListWithoutArg[LONG].insert(std::make_pair(LongOpt, false));
  m_OptListWithoutArg[SHORT].insert(std::make_pair(ShortOpt, false));  
}

void KOptions::Add(std::string LongOpt, std::string ShortOpt,
		   int OptVal){
  std::stringstream ss;
  ss << OptVal;
  m_OptPair[LONG].pair(ss.str(), false);
  m_OptPair[SHORT].pair(ss.str(), false);    
  m_OptListWithArg[LONG].insert(std::make_pair(LongOpt, m_OptPair[LONG]));
  m_OptListWithArg[SHORT].insert(std::make_pair(ShortOpt, m_OptPair[SHORT]));  
}

void KOptions::Add(std::string LongOpt, std::string ShortOpt,
		   double OptVal){
  std::stringstream ss;
  ss << OptVal;
  m_OptPair[LONG].pair(ss.str(), false);
  m_OptPair[SHORT].pair(ss.str(), false);    
  m_OptListWithArg[LONG].insert(std::make_pair(LongOpt, m_OptPair[LONG]));
  m_OptListWithArg[SHORT].insert(std::make_pair(ShortOpt, m_OptPair[SHORT]));  
}

void KOptions::Add(std::string LongOpt, std::string ShortOpt,
		   std::string OptVal){
  std::stringstream ss;
  ss << OptVal;
  m_OptPair[LONG].pair(ss.str(), false);
  m_OptPair[SHORT].pair(ss.str(), false);    
  m_OptListWithArg[LONG].insert(std::make_pair(LongOpt, m_OptPair[LONG]));
  m_OptListWithArg[SHORT].insert(std::make_pair(ShortOpt, m_OptPair[SHORT]));  
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
bool KOptions::Check(int argc, char* argv[]){ 
  std::string opt;
  for(int iarg=1; iarg<argc; iarg++){
    if(IsLongOpt(argv[iarg])){
      opt = argv[iarg];
      opt = opt.substr(2);
      if(m_OptListWithoutArg[LONG].find(opt)
	 != m_OptListWithoutArg[LONG].end()){
	m_OptListWithoutArg[LONG][opt] = true;
      }else if(m_OptListWithArg[LONG].find(opt)
	       != m_OptListWithArg[LONG].end()){
	m_OptListWithArg[LONG][opt].m_flag = true;
	if(iarg+1 < argc){
	  if(!IsOpt(argv[iarg+1])){
	    m_OptListWithArg[LONG][opt].m_val = argv[iarg+1];
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
    }else if(IsShortOpt(argv[iarg])){
      opt = argv[iarg];
      opt = opt.substr(1);
      if(m_OptListWithoutArg[SHORT].find(opt)
	 != m_OptListWithoutArg[SHORT].end()){
	m_OptListWithoutArg[SHORT][opt] = true;
      }else if(m_OptListWithArg[SHORT].find(opt)
	       != m_OptListWithArg[SHORT].end()){
	m_OptListWithArg[SHORT][opt].m_flag = true;
	if(iarg+1 < argc){
	  if(!IsOpt(argv[iarg+1])){
	    m_OptListWithArg[SHORT][opt].m_val = argv[iarg+1];
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


//bool KOptions::Exist(std::string OptName){
//  for(int i=0;i<OPT_TYPE_SIZE;i++){
//    if(std::find(m_OptListWithoutArg[i].begin(),
//		 m_OptListWithoutArg[i].end(),
//		 OptName) != m_OptListWithoutArg[i].end()) return true;
//    if((m_OptListWithArg[i].find(OptName)) != m_OptListWithArg[i].end()) return true;
//  }
//  return false;
//}

