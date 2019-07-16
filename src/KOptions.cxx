#include <KOptions.hh>
#include <utility>
#include <sstream>
#include <stdexcept>
#include <cstdio>

void KOptions::Add(std::string LongOpt, std::string ShortOpt){
  m_OptListWithoutArg[LONG].push_back(LongOpt);
  m_OptListWithoutArg[SHORT].push_back(ShortOpt);  
}

void KOptions::Add(std::string LongOpt, std::string ShortOpt,
		   int OptVal){
  std::stringstream ss;
  ss << OptVal;
  m_OptListWithArg[LONG].insert(std::make_pair(LongOpt, ss.str()));
  m_OptListWithArg[SHORT].insert(std::make_pair(ShortOpt, ss.str()));  
}

void KOptions::Add(std::string LongOpt, std::string ShortOpt,
		   double OptVal){
  std::stringstream ss;
  ss << OptVal;
  m_OptListWithArg[LONG].insert(std::make_pair(LongOpt, ss.str()));
  m_OptListWithArg[SHORT].insert(std::make_pair(ShortOpt, ss.str()));  
}

void KOptions::Add(std::string LongOpt, std::string ShortOpt,
		   std::string OptVal){
  m_OptListWithArg[LONG].insert(std::make_pair(LongOpt, OptVal));
  m_OptListWithArg[SHORT].insert(std::make_pair(ShortOpt, OptVal));  
}

int KOptions::GetI(std::string OptName){
  int val=-1;
  try{
    val=std::stoi(m_OptListWithArg[LONG].at(OptName));
  }catch (std::out_of_range& oor){
    try{
      val=std::stoi(m_OptListWithArg[SHORT].at(OptName));
    } catch (std::out_of_range& oor){
      fprintf(stderr, "Out of Range: OptList\n");
    }    
  }
  return val;
}

double KOptions::GetD(std::string OptName){
  double val=-1;
  try{
    val=std::stod(m_OptListWithArg[LONG].at(OptName));
  }catch (std::out_of_range& oor){
    try{
      val=std::stod(m_OptListWithArg[SHORT].at(OptName));
    } catch (std::out_of_range& oor){
      fprintf(stderr, "Out of Range: OptList\n");
    }    
  }
  return val;
}

float KOptions::GetF(std::string OptName){
  float val=-1;
  try{
    val=std::stof(m_OptListWithArg[LONG].at(OptName));
  }catch (std::out_of_range& oor){
    try{
      val=std::stof(m_OptListWithArg[SHORT].at(OptName));
    } catch (std::out_of_range& oor){
      fprintf(stderr, "Out of Range: OptList\n");
    }    
  }
  return val;
}

std::string KOptions::GetS(std::string OptName){
  std::string val("Hey!! Error!!");
  try{
    val=m_OptListWithArg[LONG].at(OptName);
  }catch (std::out_of_range& oor){
    try{
      val=m_OptListWithArg[SHORT].at(OptName);
    } catch (std::out_of_range& oor){
      fprintf(stderr, "Out of Range: OptList\n");
    }    
  }
  return val;
}
