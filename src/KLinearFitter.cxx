#include <KLinearFitter.hh>
#include <KNrutil.hh>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cmath>

void KLinearFitter::SetData(int nData, double *x, double *y, double *err)
{
  m_nData = nData;
  m_DataX = x; m_DataY = y; m_DataErr = err;
  m_IsConditionChange = true;
}
void KLinearFitter::AddFunction(TF1* fitfunc)
{
  m_Func.push_back(fitfunc);
  m_CoeffMin.push_back(0);
  m_CoeffMax.push_back(0);
  m_SetParLimits.push_back(false);
  m_IsConditionChange = true;
}
void KLinearFitter::AddFunction(std::vector<TF1*>& fitfunlist)
{
  for(const auto& x : fitfunlist) m_Func.push_back(x);
  for(std::size_t idx = 0; idx != fitfunlist.size(); ++idx){
    m_CoeffMin.push_back(0); m_CoeffMax.push_back(0);
    m_SetParLimits.push_back(false);
  }
  m_IsConditionChange = true;  
}
void KLinearFitter::SetFuncList(std::vector<TF1*>& fitfunlist)
{
  m_Func.clear();
  for(const auto& x : fitfunlist) m_Func.push_back(x);
  m_CoeffMin.resize(fitfunlist.size(), 0); m_CoeffMax.resize(fitfunlist.size(), 0);
  m_SetParLimits.resize(fitfunlist.size(), false);
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
    m_nTotalFit = (1 << m_Func.size()); // the maximum number of times for fitting
    m_nTotalFit--;
    m_IsConditionChange = false;
  }
  if(m_Svd){
    double tmpchisq;
    for(int ifit = 1; ifit != m_nTotalFit + 1; ++ifit){
      ChooseFitfunc(ifit);
      KNrutil::svdfit(m_DataX, m_DataY, m_DataErr, m_nData,
		      &m_Coeff[0], (int)m_Coeff.size(), m_u, m_v,
		      m_w, m_Fitfunc, &m_Chisquare, function);
      MakeCoefficient(ifit);
      std::cout << ifit << " " << m_Chisquare << " ";
      for(int ipar = 0; ipar != (int)m_Coeff.size(); ++ipar)
 	std::cout << m_Coeff[ipar] << " ";
      std::cout << std::endl;
      if(!CheckParRange()) continue;      
      if(ifit == 1) tmpchisq = m_Chisquare;
      else{
	if(m_Chisquare > tmpchisq) continue;
	tmpchisq = m_Chisquare; //candidate for minimum combination
	m_iMinChisq = ifit;
      }      
    }
    ChooseFitfunc(m_iMinChisq);
    KNrutil::svdfit(m_DataX, m_DataY, m_DataErr, m_nData,
		    &m_Coeff[0], (int)m_Coeff.size(), m_u, m_v,
		    m_w, m_Fitfunc, &m_Chisquare, function);
    KNrutil::svdvar(m_v, (int)m_Coeff.size(), m_w, m_cvm); 
    MakeCoefficient(m_iMinChisq);
    MakeDeviation(m_iMinChisq);

    std::cout << m_iMinChisq << std::endl;
  }
}

void KLinearFitter::function(double x, double *afunc, std::vector<TF1*>& funclist)
{
  for(int i = 0; i != funclist.size(); ++i) afunc[i] = funclist[i]->Eval(x);
}

void KLinearFitter::SetParLimits(int ipar, double min, double max)
{
  if(ipar > (int)m_Func.size()){
    std::cout << "KLinearFitter: Invalid number of parameter" << std::endl;
    std::cout << "Abort." << std::endl;
  }
  m_CoeffMin[ipar] = min; m_CoeffMax[ipar] = max;
  m_SetParLimits[ipar] = true;
}


bool KLinearFitter::CheckParRange()
{
  for(std::size_t ipar = 0; ipar != m_Coeff.size(); ++ipar)
    if(m_SetParLimits[ipar])
      if(m_Coeff[ipar] < m_CoeffMin[ipar] || m_Coeff[ipar] > m_CoeffMax[ipar]) return false;
  return true;
}

void KLinearFitter::ChooseFitfunc(int ifit)
{
  m_Fitfunc.clear();
  for(int i = 0; i != (int)m_Func.size(); ++i)
    if((ifit>>i)&1) m_Fitfunc.push_back(m_Func[i]);
  int nfitfun = (int)m_Fitfunc.size();
  m_u.resize(m_nData);
  for(auto&& x : m_u) x.resize(nfitfun);
  m_v.resize(nfitfun);
  for(auto&& x : m_v) x.resize(nfitfun);
  m_w.resize(nfitfun);
  m_cvm.resize(nfitfun);
  for(auto&& x : m_cvm) x.resize(nfitfun);
  m_Coeff.resize(nfitfun);
}

void KLinearFitter::ReleaseParameter(int ipar)
{
  if(ipar > (int)m_SetParLimits.size()){
    std::cout << "KLinearFitter: Invalid number of parameter" << std::endl;
    std::cout << "Abort." << std::endl;
  }
  m_SetParLimits[ipar] = false;
}

void KLinearFitter::MakeCoefficient(int ifit)
{
  // post-processing for make coefficient vector
  auto tmp = m_Coeff;
  int k = 0;
  m_Coeff.clear();
  m_Coeff.resize(m_Func.size(), 0);
  for(int i = 0; i != (int)m_Func.size(); ++i)
    if((ifit>>i)&1) m_Coeff[i] = tmp[k++];
}

void KLinearFitter::MakeDeviation(int ifit)
{
  auto tmpcvm = m_cvm;
  std::vector<double> v(tmpcvm.size()*tmpcvm.size());
  int k = 0;
  for(int i = 0; i != (int)tmpcvm.size(); ++i)
    for(int j = 0; j != (int)tmpcvm.size(); ++j)
      v[k++] = tmpcvm[i][j]; // 2d matrix to 1d vector
  m_cvm.clear();
  m_cvm.resize(m_Func.size()); 
  for(auto &&x : m_cvm) x.resize(m_Func.size(), 0); // initialize output
  k = 0;
  for(int i = 0; i != (int)m_Func.size(); ++i)
    for(int j = 0; j != (int)m_Func.size(); ++j)
      if((ifit>>i)&1) if((ifit>>j)&1) m_cvm[i][j] = v[k++];
  m_dev.clear();
  m_dev.resize(m_Func.size(), 0);
  for(int i = 0; i != m_Func.size(); ++i) m_dev[i] = m_cvm[i][i];
}
