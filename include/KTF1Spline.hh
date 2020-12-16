#ifndef K_TF1SPLINE_HH
#define K_TF1SPLINE_HH

#include <KTheodata.hh>
#include <TF1.h>
#include <vector>

class KTF1Spline{
private:
  TF1 *f;
  static double Spline(double *, double*); // spline function

public:
  KTF1Spline();
  KTF1Spline(const char *name, const KTheodata &Theo, bool normalization = false);
  // if false, function cannot be normalized (magnitude is fixed)
  KTF1Spline(const char *name, const std::vector<double>& vx,
	     const std::vector<double>& vy, bool normalization = false);
  // elements in x and y must be ordered.
  KTF1Spline(const char *name, const int n, const double *x, const double *y,
	     bool normalization = false);    
  ~KTF1Spline();

  void SetData(const char *name, const KTheodata &Theo, bool normalization = false);
  void SetData(const char *name, const std::vector<double>& vx,
	       const std::vector<double>& vy, bool normalization = false);  
  void SetData(const char *name, const int n, const double *x, const double *y,
	       bool normalization = false);
  void Clear();

public:
  inline TF1* Getf(){return f;}
};

#endif
