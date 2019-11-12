#ifndef _KUTIL_HH
#define _KUTIL_HH

#include <K3Vector.hh>

#define SIZEOF(ARRAY) sizeof(array)/sizeof((array)[0])

namespace KUtil {
  void Normalize(double norm, double& x, double& y, double& z);
  void Normalize(double norm, double& x, double& y);
  void Normalize(double norm, K3Vector &p);
  double BetaToGamma(double beta);
  double GammaToBeta(double gamma);    
}

#endif
