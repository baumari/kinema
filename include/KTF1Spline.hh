#ifndef K_TF1SPLINE_HH
#define K_TF1SPLINE_HH

#include <KTheodata.hh>
#include <TF1.h>

class KTF1Spline{
private:
  TF1 *f;
  static double Spline(double *, double*); // spline function

public:
  KTF1Spline(const char *name, KTheodata &Theo, bool normalization = false);
  // if false, function cannot be normalized (magnitude is fixed)
  ~KTF1Spline();

public:
  inline TF1* Getf(){return f;}
};

#endif
