#ifndef _KUTIL_HH
#define _KUTIL_HH

#define SIZEOF(ARRAY) sizeof(array)/sizeof((array)[0])

namespace KUtil {
  void Normalize(double norm, double& x, double& y, double& z);
  void Normalize(double norm, double& x, double& y);
}

#endif
