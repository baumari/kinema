#include <cmath>
#include <iostream>
#include <cstdio>
#include <KUtil.hh>

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
}
