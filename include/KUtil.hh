#ifndef K_UTIL_HH
#define K_UTIL_HH

#include <K3Vector.hh>
#include <limits>
#include <cmath>
#include <string>
#include <vector>

#define SIZEOF(ARRAY) sizeof(ARRAY)/sizeof((ARRAY)[0])
namespace KUtil {
  const double EPSILON = std::numeric_limits<double>::epsilon();
  const double LOOSE_EPSILON = 1e-5;
  const double MAX = std::numeric_limits<double>::max();
  const double LOOSE_MAX = 1e5;
  const double C = 299792458; // m/s
  const double HBAR = 6.582119569e-16; //eV*s
  const double HBARC = C*HBAR*1e9; //MeV*fm
  const double ALPHA = 1./137.036; // fine structure constant
  const double NA = 6.022e23; //avogadro number
  const double AMU = 931.478; // in MeV/c**2
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
  std::string str_tolower(std::string s);
  std::string str_toupper(std::string s);
  double Spline(double *x, double *par);
  // split("hoge.txt.fuga",'.') -> [0]: hoge, [1]: txt, [2]: fuga
  std::vector<std::string> split(const std::string &s, char delim); 
}

#endif
