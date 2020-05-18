#include <cmath>
#include <iostream>
#include <cstdio>
#include <KUtil.hh>
#include <cstdlib>
#include <stdarg.h>
#include <cctype>
#include <string>
#include <algorithm>
#include <vector>
#include <TSpline.h>

namespace KUtil {
  void Normalize(double norm,
	    double& x, double& y, double& z) // normalization 
  {
    double fac=norm/sqrt(x*x+y*y+z*z);
    x*=fac; y*=fac; z*=fac;
  } 
  void Normalize(double norm,
	    double& x, double& y)
  {
    double fac=norm/sqrt(x*x+y*y);
    x*=fac; y*=fac;  
  }
  void Normalize(double norm, K3Vector &p)
  {
    double fac = norm/p.Norm();
    p.Set(p.X()*fac, p.Y()*fac, p.Z()*fac);
  }

  double BetaToGamma(double beta)
  {
    if(beta >= 1){
      fprintf(stderr, "Error: Called by BetaToGamma()..\n");
      fprintf(stderr, "Beta must be less than 1!!\n");
      return EXIT_FAILURE;
    }
    return 1./sqrt(1-pow(beta,2));
  }

  double BetaToGamma(const K3Vector& beta)
  {
    if(beta.Norm() >= 1){
      fprintf(stderr, "Error: Called by BetaToGamma()..\n");
      fprintf(stderr, "Beta must be less than 1!!\n");
      return EXIT_FAILURE;      
    }
    return 1./sqrt(1-pow(beta.Norm(),2));
  }

  double GammaToBeta(double gamma)
  {
    if(gamma <= 0){
      fprintf(stderr, "Error: Called by GammaToBeta()..\n");
      fprintf(stderr, "Gamma must be larger than 0!!\n");
      return EXIT_FAILURE;
    }
    return sqrt(1.-1./pow(gamma,2));
  }
  
  std::string str_tolower(std::string s){
//    std::transform(s.begin(), s.end(), s.begin(),
//		   [](unsigned char c){return std::tolower(c);}
//		   );
    return s;
  }
  std::string str_toupper(std::string s){
//    std::transform(s.begin(), s.end(), s.begin(),
//		   [](unsigned char c){return std::toupper(c);}
//		   );
    return s;
  }  
  double Spline(double *x, double *par){
    double xx = x[0];
    int nData = par[0];
    std::vector<double> xn, yn;
    xn.resize(nData); yn.resize(nData);
    for(int idx = 0; idx != nData; ++idx){
      xn[idx] = par[idx+1];
      yn[idx] = par[idx+1+nData];
    }
    TSpline3 sp3("sp3", &xn[0], &yn[0], nData);
    return sp3.Eval(xx)*par[2*nData+1];
  }
}
