#include <KLinearFitter.hh>
#include <KNrutil.hh>
#include <cstring>
#include <iostream>
#include <algorithm>

void KLinearFitter::SetData(int nData, double *x, double *y, double *err)
{
  m_nData = nData;
  m_DataX = x; m_DataY = y; m_DataErr = err;
  m_IsConditionChange = true;
}
void KLinearFitter::AddFunction(TF1* fitfunc)
{
  m_Func.push_back(fitfunc);
  m_IsConditionChange = true;
}
void KLinearFitter::AddFunction(std::vector<TF1*>& fitfunlist)
{
  m_Func.clear(); m_Func.resize(fitfunlist.size());
  std::copy(fitfunlist.begin(), fitfunlist.end(), m_Func.begin());
  m_IsConditionChange = true;  
}
void KLinearFitter::Fit(const char *method)
{
  if(m_nData == 0 || m_DataX == nullptr || m_DataY == nullptr
     || m_DataErr == nullptr){
    std::cout << "KLinearFitter: Invalid input data." << std::endl;
    std::cout << "Abort." << std::endl;
    return ;    
  }
  if(m_Func.size() == 0){
    std::cout << "KLinearFitter: No fitting function." << std::endl;
    std::cout << "Abort." << std::endl;
    return ;        
  }
  if(strcmp(method,"svd") == 0) m_Svd = true;
  else if(strcmp(method,"normeqn") == 0) m_Svd = false;
  else{
    std::cout << "KLinearFitter: Unknown fitting method." << std::endl;
    std::cout << "Abort." << std::endl;
    return ;
  }
  if(m_IsConditionChange){
    m_u.resize(m_nData);
    for(auto&& x : m_u) x.resize(m_Func.size());
    m_v.resize(m_Func.size());
    for(auto&& x : m_v) x.resize(m_Func.size());    
    m_w.resize(m_Func.size());
    m_cvm.resize(m_Func.size());
    for(auto&& x : m_cvm) x.resize(m_Func.size());
    m_dev.resize(m_Func.size());
    m_IsConditionChange = false;
  }
  if(m_Svd){
    KNrutil::svdfit(m_DataX, m_DataY, m_DataErr, m_nData,
		    &m_Coeff[0], m_Func.size(), m_u, m_v,
		    m_w, m_Func, &m_Chisquare, function);
    KNrutil::svdvar(m_v, m_Func.size(), m_w, m_cvm);
    for(std::size_t idx = 0; idx != m_dev.size(); ++idx){
      m_dev[idx] = m_cvm[idx][idx];
    }    
  }
}

void KLinearFitter::function(double x, double *afunc, std::vector<TF1*>& funclist)
{
  for(int i = 0; i != funclist.size(); ++i) afunc[i] = funclist[i]->Eval(x);
}
