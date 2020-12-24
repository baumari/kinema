#include <KLinearFitter.hh>
#include <KNrutil.hh>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <KUtil.hh>

#define DEBUG

void KLinearFitter::SetData(int nData, const double *x, const double *y, const double *err)
{
  if(m_DataX){
    delete[] m_DataX;
    m_DataX = nullptr;
  }
  if(m_DataY){
    delete[] m_DataY;
    m_DataY = nullptr;
  }
  if(m_DataErr){
    delete[] m_DataErr;
    m_DataErr = nullptr;
  }    
  m_nData = nData;
  m_DataX = new double[nData];
  m_DataY = new double[nData];
  m_DataErr = new double[nData];
  for(int i = 0; i != nData; ++i){
    m_DataX[i] = x[i];
    m_DataY[i] = y[i];
    m_DataErr[i] = err[i];
  }    
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
  else if(strcmp(method,"root") == 0) m_Svd = false;
  else{
    std::cout << "KLinearFitter: Unknown fitting method." << std::endl;
    std::cout << "Abort." << std::endl;
    return ;
  }  
  m_NoSolution = false;        
  if(m_Svd){
    if(m_IsConditionChange){
      m_nTotalFit = (1 << m_Func.size()); // the maximum number of times for fitting
      m_nTotalFit--;
      MakeTheoData();
      MakeExpData(); // correction for exp data (fix, release)
      m_IsConditionChange = false;
    }    
    double tmpchisq;
    bool First = true;
    for(int ifit = 1; ifit != m_nTotalFit + 1; ++ifit){
      if(ifit&m_FixParameter) continue;
      ChooseFitfunc(ifit);
      KNrutil::svdfit(m_DataY, m_DataErr, m_nData,
		      &m_a[0], (int)m_a.size(), m_u, m_v,
		      m_w, m_Fitfunc, &m_Chisquare, function);
      MakeCoefficient(ifit);
#ifdef DEBUG
      std::cout << "ifit: " << ifit << ", chisq: " << m_Chisquare << " ";
      //      for(const auto &x : m_Coeff) std::cout << x << " ";
      std::cout << std::endl;
#endif
      if(!CheckParRange(ifit)) continue;
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
#ifdef DEBUG
    std::cout << "m_iMinChisq: " << m_iMinChisq
	      << " m_FixParameter: " << m_FixParameter << std::endl;
#endif
    if(First){ // no solution found in the ParRange
      m_NoSolution = true;      
      return ;
    }      
    ChooseFitfunc(m_iMinChisq);
    KNrutil::svdfit(m_DataY, m_DataErr, m_nData,
		    &m_a[0], (int)m_a.size(), m_u, m_v,
		    m_w, m_Fitfunc, &m_Chisquare, function);
    KNrutil::svdvar(m_v, (int)m_a.size(), m_w, m_cvm); 
    MakeCoefficient(m_iMinChisq);
    MakeDeviation(m_iMinChisq);
  }else{ // ROOT method (minuit)
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


bool KLinearFitter::CheckParRange(int ifit)
{
  for(std::size_t ipar = 0; ipar != m_Coeff.size(); ++ipar){
    if((ifit>>ipar)&1){
      if(m_SetParLimits[ipar]){
	if(m_Coeff[ipar] <  m_CoeffMin[ipar] || m_Coeff[ipar] > m_CoeffMax[ipar])
	  return false;	
      }
    }
  }
  return true;
}

void KLinearFitter::ChooseFitfunc(int ifit)
{
  m_Fitfunc.clear();
  for(int i = 0; i != (int)m_Func.size(); ++i){
    if((ifit>>i)&1){
      if(!((m_FixParameter>>i)&1)) m_Fitfunc.push_back(m_TheoY.at(i));      
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
    m_DataY[idx] = m_DataYOrg.at(idx);
  }
  // subtraction of exp data from fixed theoretical data
  for(std::size_t ifunc = 0; ifunc != m_Func.size(); ++ifunc){
    if((m_FixParameter>>ifunc)&1){
      for(std::size_t idx = 0; idx != m_nData; ++idx){ // subtract base function 
	m_DataY[idx] -= m_Coeff.at(ifunc)*m_TheoY.at(ifunc).at(idx);
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
  m_FixParameter = (m_FixParameter & ~(1<<ipar));
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
  m_FixParameter = (m_FixParameter | (1<<ipar));
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
  if(m_CoeffMin.size() == 0 || m_CoeffMax.size() == 0){
    std::cout << "KLinearFitter: "
      "Parameter range must be specified." << std::endl;
    return ;
  }
  for(int i = 0; i != m_nData; ++i) // mizumashi error
     m_DataErr[i] *= sqrt(GetReducedChisquare());
  Fit();
  if(m_Func.size() == 1) return ;
  int count = 0; // counter for binary search
  double bin_min, bin_max;
  int zero = 0;
  double delta = 0.05;
  auto MinCoeff = m_Coeff;
  auto MinChisq = GetChisquare();
  auto tmpchisq = MinChisq;
  m_ChisqLog.resize(m_Coeff.size()); m_CoeffLog.resize(m_Coeff.size());
  m_CoeffErrMin.resize(m_Coeff.size()); m_CoeffErrMax.resize(m_Coeff.size());
  bool lower_limit = false;
  bool higher_limit = false;
  // init parameters
  for(int ipar = 0; ipar != m_Func.size(); ++ipar){
    ReleaseParameter(ipar);
    SetParLimits(ipar, m_CoeffMin[ipar], m_CoeffMax[ipar]);
  }    
  for(std::size_t ifunc = 0; ifunc != m_Func.size(); ++ifunc){ // binary search
    if(fabs(MinCoeff[ifunc]-m_CoeffMin[ifunc]) < KUtil::EPSILON){
      lower_limit = true;
    }
    if(!lower_limit){
      while(tmpchisq < MinChisq + 1){ // determination of lower range
	ReleaseParameter(ifunc);
	if(count == 0){ // guarantee at least one calculated point
	  FixParameter(ifunc, MinCoeff[ifunc]+1e-10);
	}
	else{
	  FixParameter(ifunc, MinCoeff[ifunc]
		       -delta*(MinCoeff[ifunc]+1e-10)*pow(2.,count-1));
	}
	count++;
	if(GetParameter(ifunc) < m_CoeffMin[ifunc]){
	  zero = 1;
	  break;
	}

	Fit();
	if(NonSolution()){
	  //	  std::cout << "Nonsolution min: " << ifunc << std::endl;
	  tmpchisq = GetChisquareForce();
	  m_ChisqLog[ifunc].push_back(tmpchisq); // log
	  m_CoeffLog[ifunc].push_back(GetParameter(ifunc)); // log
	  continue;
	}
	tmpchisq = GetChisquare();
	m_ChisqLog[ifunc].push_back(tmpchisq); // log
	m_CoeffLog[ifunc].push_back(GetParameter(ifunc)); // log      
      }
      if(zero){
	bin_min = m_CoeffMin[ifunc]; // initialization of binary search 
	bin_max = MinCoeff[ifunc];	
      }else{
	bin_min = GetParameter(ifunc); // initialization of binary search 
	bin_max = MinCoeff[ifunc];	
      }
      if(fabs(bin_min - bin_max) < KUtil::LOOSE_EPSILON){ 
	m_CoeffErrMin[ifunc] = bin_min;
      }else{
	while(fabs(tmpchisq-(MinChisq+1)) > 1e-03){
	  if(fabs(bin_min - bin_max) < KUtil::EPSILON){
	    std::cout << "May not be converged.. Check Log vector."
		      << " (ifunc = " << ifunc << "), min" << std::endl;
	    m_CoeffErrMin[ifunc] = bin_min;
	    break;
	  }
	  ReleaseParameter(ifunc);
	  FixParameter(ifunc, (bin_max+bin_min)/2.);
	  Fit();
	  if(NonSolution()){
	    tmpchisq = GetChisquareForce();
	    if(tmpchisq > MinChisq+1) bin_min = (bin_min+bin_max)/2.;
	    else bin_max = (bin_min+bin_max)/2.;
	    m_ChisqLog[ifunc].push_back(tmpchisq); // log
	    m_CoeffLog[ifunc].push_back(GetParameter(ifunc)); // log	    
	    continue;
	  }
	  tmpchisq = GetChisquare();
	  if(tmpchisq > MinChisq+1) bin_min = (bin_min+bin_max)/2.;
	  else bin_max = (bin_min+bin_max)/2.;
	  m_ChisqLog[ifunc].push_back(tmpchisq); // log
	  m_CoeffLog[ifunc].push_back(GetParameter(ifunc)); // log
	}
	m_CoeffErrMin[ifunc] = GetParameter(ifunc);	
      }
    }else{ // if(lower_limit)
      m_CoeffErrMin[ifunc] = MinCoeff[ifunc];
    }// determination of lower range here  
    ReleaseParameter(ifunc);
    count = 0; zero = 0;
    tmpchisq = MinChisq;
    if(fabs(MinCoeff[ifunc]-m_CoeffMax[ifunc]) < KUtil::EPSILON){
      higher_limit = true;
    }
    if(!higher_limit){
      while(tmpchisq < MinChisq + 1){ // determination of lower range
	ReleaseParameter(ifunc);
	if(count == 0){ // guarantee at least one calculated point
	  FixParameter(ifunc, MinCoeff[ifunc]+1e-10);
	}
	else{
	  FixParameter(ifunc, MinCoeff[ifunc]
		       +delta*(MinCoeff[ifunc]+1e-10)*pow(2.,count-1));
	}
	count++;
	if(GetParameter(ifunc) > m_CoeffMax[ifunc]){
	  zero = 1;
	  break;
	}
	Fit();
	if(NonSolution()){
	  //	  std::cout << "Nonsolution max: " << ifunc << std::endl;
	  tmpchisq = GetChisquareForce();
	  m_ChisqLog[ifunc].push_back(tmpchisq); // log
	  m_CoeffLog[ifunc].push_back(GetParameter(ifunc)); // log	  
	  continue;
	}
	tmpchisq = GetChisquare();
	m_ChisqLog[ifunc].push_back(tmpchisq); // log
	m_CoeffLog[ifunc].push_back(GetParameter(ifunc)); // log      
      }
      if(zero){
	bin_min = MinCoeff[ifunc];	
	bin_max = m_CoeffMax[ifunc]; // initialization of binary search
      }else{
	bin_min = MinCoeff[ifunc];	
	bin_max = GetParameter(ifunc); // initialization of binary search 
      }
      if(fabs(bin_min - bin_max) < KUtil::LOOSE_EPSILON){ 
	m_CoeffErrMax[ifunc] = bin_max;
      }else{
	while(fabs(tmpchisq-(MinChisq+1)) > 1e-03){
	  if(fabs(bin_min - bin_max) < KUtil::EPSILON){
	    std::cout << "May not be converged.. Check Log vector."
		      << " (ifunc = " << ifunc << "), max" << std::endl;
	    m_CoeffErrMax[ifunc] = bin_max;
	    break;
	  }
	  ReleaseParameter(ifunc);
	  FixParameter(ifunc, (bin_max+bin_min)/2.);
	  Fit();
	  if(NonSolution()){
	    tmpchisq = GetChisquareForce();
	    if(tmpchisq > MinChisq+1) bin_max = (bin_min+bin_max)/2.;
	    else bin_min = (bin_min+bin_max)/2.;
	    m_ChisqLog[ifunc].push_back(tmpchisq); // log
	    m_CoeffLog[ifunc].push_back(GetParameter(ifunc)); // log	  
	    continue;
	  }
	  tmpchisq = GetChisquare();
	  if(tmpchisq > MinChisq+1) bin_max = (bin_min+bin_max)/2.;
	  else bin_min = (bin_min+bin_max)/2.;
	  m_ChisqLog[ifunc].push_back(tmpchisq); // log
	  m_CoeffLog[ifunc].push_back(GetParameter(ifunc)); // log
	}
	m_CoeffErrMax[ifunc] = GetParameter(ifunc);	
      }
    }else{ // if(higher_limit)
      m_CoeffErrMax[ifunc] = MinCoeff[ifunc];
    }// determination of higher range here
  
    count = 0; zero = 0;
    ReleaseParameter(ifunc);
    tmpchisq = MinChisq;
    lower_limit = false;
    higher_limit = false;
    SetParLimits(ifunc, m_CoeffMin[ifunc], m_CoeffMax[ifunc]);
  }
  for(int ifunc = 0; ifunc != m_Coeff.size(); ++ifunc) m_Coeff[ifunc] = MinCoeff[ifunc];  
}

double KLinearFitter::GetChisquareForce()
{
  double chisq = 0;
  for(int idx = 0; idx != m_nData; ++idx){
    chisq += (m_DataY[idx]/m_DataErr[idx])*(m_DataY[idx]/m_DataErr[idx]);
  }
  return chisq;
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

int KLinearFitter::GetNParFixed()
{
  int npar = 0;
  for(int ipar = 0; ipar != m_Func.size(); ++ipar)
    if((m_FixParameter>>ipar)&1) npar++;
  return npar;
}

void KLinearFitter::Clear()
{
  m_Chisquare = 0;
  m_nData = 0;
  m_nTotalFit = 1;
  m_iMinChisq = 1;
  if(m_DataX){
    delete[] m_DataX;
    m_DataX = nullptr;
  }
  if(m_DataY){
    delete[] m_DataY;
    m_DataY = nullptr;
  }
  if(m_DataErr){
    delete[] m_DataErr;
    m_DataErr = nullptr;
  }    
  m_TheoY.clear();
  m_DataYOrg.clear();
  m_Coeff.clear();
  m_CoeffMin.clear(); m_CoeffMax.clear();
  m_CoeffErrMin.clear(); m_CoeffErrMax.clear();
  m_Svd = false;
  m_Error = false;
  m_IsConditionChange = false;
  m_NoSolution = false;
  m_SetParLimits.clear();
  m_FixParameter = 0;
  m_u.clear(); m_v.clear();
  m_w.clear(); m_cvm.clear();
  m_dev.clear();
  m_a.clear();
  m_Func.clear();
  m_Fitfunc.clear();
  m_ChisqLog.clear();  m_CoeffLog.clear();
}
