#ifndef K_TF1SPLINE_HH
#define K_TF1SPLINE_HH

#include <KTheodata.hh>
#include <TF1.h>

class KTF1Spline{
private:
  TF1 *f;

public:
  KTF1Spline(const char *name, double(*func)(double *, double *), KTheodata &Theo, bool normalization = false);
  // if false, function cannot be normalized (magnitude is fixed)
  ~KTF1Spline();

public:
  TF1* Getf();
};

#endif
