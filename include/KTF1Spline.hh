#ifndef _KTF1SPLINE_HH
#define _KTF1SPLINE_HH

#include <KTheodata.hh>
#include <TF1.h>

class KTF1Spline{
private:
  TF1 *f;

public:
  KTF1Spline(const char *name, double(*func)(double *, double *), KTheodata &Theo);
  ~KTF1Spline();
};

#endif
