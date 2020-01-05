#ifndef _KUTIL_HH
#define _KUTIL_HH

#include <K3Vector.hh>
#include <limits>

#define SIZEOF(ARRAY) sizeof(ARRAY)/sizeof((ARRAY)[0])
namespace KUtil {
  const double EPSILON = std::numeric_limits<double>::epsilon();
}
namespace KUtil {
  void Normalize(double norm, double& x, double& y, double& z);
  void Normalize(double norm, double& x, double& y);
  void Normalize(double norm, K3Vector &p);
  double BetaToGamma(double beta);
  double BetaToGamma(const K3Vector& beta);  
  double GammaToBeta(double gamma);
}


#endif
