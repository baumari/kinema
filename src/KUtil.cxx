#include <cmath>
#include <iostream>
#include <cstdio>
#include <KUtil.hh>
#include <cstdlib>

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
}
