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
  m_DataYOrg.clear(); 
  for(std::size_t idx = 0; idx != nData; ++idx){
    m_DataYOrg.push_back(y[idx]);
  }
  m_IsConditionChange = true;
}
void KLinearFitter::AddFunction(TF1* fitfunc)
{
  m_Func.push_back(fitfunc);
  m_Coeff.push_back(0);
  m_CoeffMin.push_back(0);
  m_CoeffMax.push_back(0);
  m_SetParLimits.push_back(false);
  m_IsConditionChange = true;
}
void KLinearFitter::AddFunction(std::vector<TF1*>& fitfunlist)
{
  for(const auto& x : fitfunlist) m_Func.push_back(x);
  for(std::size_t idx = 0; idx != fitfunlist.size(); ++idx){
    m_Coeff.push_back(0); m_CoeffMin.push_back(0); m_CoeffMax.push_back(0);
    m_SetParLimits.push_back(false); 
  }
  m_IsConditionChange = true;  
}
void KLinearFitter::SetFuncList(std::vector<TF1*>& fitfunlist)
{
  m_Func.clear();
  for(const auto& x : fitfunlist) m_Func.push_back(x);
  m_Coeff.resize(fitfunlist.size(), 0);
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
    MakeTheoData();
    MakeExpData(); // correction for exp data (fix, release)
    m_IsConditionChange = false;
  }
  if(m_Svd){
    double tmpchisq;
    bool First = true;
    for(int ifit = 1; ifit != m_nTotalFit + 1; ++ifit){
      if(ifit&m_FixParameter) continue;
      ChooseFitfunc(ifit);
      KNrutil::svdfit(m_DataY, m_DataErr, m_nData,
		      &m_a[0], (int)m_a.size(), m_u, m_v,
		      m_w, m_Fitfunc, &m_Chisquare, function);
      MakeCoefficient(ifit);
      std::cout << ifit << " " << m_Chisquare << " ";
      for(const auto &x : m_Coeff) std::cout << x << " ";
      std::cout << std::endl;
      if(!CheckParRange()) continue;      
      if(First){
	tmpchisq = m_Chisquare;
	m_iMinChisq = ifit;
	First = false;
      }
      else{
	if(m_Chisquare > tmpchisq) continue;
	tmpchisq = m_Chisquare; //candidate for minimum combination
	m_iMinChisq = ifit;
      }      
    }
    std::cout << m_iMinChisq << std::endl;	  
    ChooseFitfunc(m_iMinChisq);
    KNrutil::svdfit(m_DataY, m_DataErr, m_nData,
		    &m_a[0], (int)m_a.size(), m_u, m_v,
		    m_w, m_Fitfunc, &m_Chisquare, function);
    KNrutil::svdvar(m_v, (int)m_a.size(), m_w, m_cvm); 
    MakeCoefficient(m_iMinChisq);
    MakeDeviation(m_iMinChisq);
  }
}

void KLinearFitter::function(int ix, double *afunc,
			     std::vector<std::vector<double> >& TheoY)
{
  for(std::size_t ifunc = 0; ifunc != TheoY.size(); ++ifunc){
    afunc[ifunc] = TheoY[ifunc][ix];
  }
}

void KLinearFitter::SetParLimits(int ipar, double min, double max)
{
  if(ipar > (int)m_Func.size()){
    std::cout << "KLinearFitter: Invalid number of parameter" << std::endl;
    std::cout << "Abort." << std::endl;
    std::exit(EXIT_FAILURE);    
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
  for(int i = 0; i != (int)m_Func.size(); ++i){
    if((ifit>>i)&1){
      if(!((m_FixParameter>>i)&1)) m_Fitfunc.push_back(m_TheoY[i]);      
    }
  }
  int nfitfun = (int)m_Fitfunc.size();
  m_u.resize(m_nData);
  for(auto&& x : m_u) x.resize(nfitfun);
  m_v.resize(nfitfun);
  for(auto&& x : m_v) x.resize(nfitfun);
  m_w.resize(nfitfun);
  m_cvm.resize(nfitfun);
  for(auto&& x : m_cvm) x.resize(nfitfun);
  m_a.resize(nfitfun);
}

void KLinearFitter::MakeExpData()
{
  // initialization of exp data
  for(std::size_t idx = 0; idx != m_nData; ++idx){
    m_DataY[idx] = m_DataYOrg[idx];
  }
  // subtraction of exp data from fixed theoretical data
  for(std::size_t ifunc = 0; ifunc != m_Func.size(); ++ifunc){
    if((m_FixParameter>>ifunc)&1){
      for(std::size_t idx = 0; idx != m_nData; ++idx){ // subtract base function 
	m_DataY[idx] -= m_Coeff[ifunc]*m_TheoY[ifunc][idx];
      }      
    }
  }
}

void KLinearFitter::ReleaseParameter(int ipar)
{
  if(ipar > (int)m_SetParLimits.size()){
    std::cout << "KLinearFitter: Invalid number of parameter" << std::endl;
    std::cout << "Abort." << std::endl;
    return ;
  }
  m_SetParLimits[ipar] = false;
  m_FixParameter -= (1<<ipar);
  m_IsConditionChange = true;
}

void KLinearFitter::MakeCoefficient(int ifit)
{
  // post-processing for make coefficient vector
  int k = 0;
  for(int i = 0; i != (int)m_Func.size(); ++i){
    if(!((m_FixParameter>>i)&1)){
      m_Coeff[i] = 0;
      if((ifit>>i)&1) m_Coeff[i] = m_a[k++];
    }
  }
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

std::vector<double> KLinearFitter::GetParErrors()
{
  std::vector<double> tmp;
  for(const auto& x : m_dev) tmp.push_back(sqrt(x));
  return tmp;
}

void KLinearFitter::FixParameter(int ipar, double val)
{
  if(ipar > (int)m_Func.size()){
    std::cout << "KLinearFitter: Invalid number of parameter" << std::endl;
    std::cout << "Abort." << std::endl;
    std::exit(EXIT_FAILURE);        
  }
  m_FixParameter += (1<<ipar);
  m_Coeff[ipar] = val;
  m_IsConditionChange = true;
}

void KLinearFitter::ErrorEstimationByChisquare()
{
  if(m_dev.size() == 0){
    std::cout << "KLinearFitter: "
      "Default fitting must be perfomed before this routine is called." << std::endl;
    return ;
  }
  for(int i = 0; i != m_nData; ++i)
    m_DataErr[i] *= sqrt(GetReducedChisquare());
  if(m_Func.size() == 1) Fit();
  double count = 0.0; // counter for binary search
  double bin_min, bin_max;
  double dchisq;
  int zero = 0;
  double delta = 0.2;
  //  for(std::size_t ifunc = 0; ifunc != m_Fitfun.size(); ++ifunc){
  //    do{
      
  //    }
  //  }
  
}

void KLinearFitter::MakeTheoData()
{
  m_TheoY.clear();
  m_TheoY.resize(m_Func.size());
  for(auto&& x : m_TheoY) x.resize(m_nData, 0);
  for(std::size_t ifunc = 0; ifunc != m_Func.size(); ++ifunc){
    for(std::size_t idata = 0; idata != m_nData; ++idata){
      m_TheoY[ifunc][idata] = m_Func[ifunc]->Eval(m_DataX[idata]);
    }
  }
}
