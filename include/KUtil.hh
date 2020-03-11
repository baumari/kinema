#ifndef _KUTIL_HH
#define _KUTIL_HH

#include <K3Vector.hh>
#include <limits>
#include <cmath>

#define SIZEOF(ARRAY) sizeof(ARRAY)/sizeof((ARRAY)[0])
namespace KUtil {
  const double EPSILON = std::numeric_limits<double>::epsilon();
  const double LOOSE_EPSILON = 1e-5;
  const double MAX = std::numeric_limits<double>::max();
  const double LOOSE_MAX = 1e5;
}
namespace KUtil {
  void Normalize(double norm, double& x, double& y, double& z);
  void Normalize(double norm, double& x, double& y);
  void Normalize(double norm, K3Vector &p);
  double BetaToGamma(double beta);
  double BetaToGamma(const K3Vector& beta);  
  double GammaToBeta(double gamma);
  inline double RadToDeg(double radian){return 180./M_PI*radian;}
  inline double DegToRad(double degree){return M_PI/180.*degree;}
}


#endif
