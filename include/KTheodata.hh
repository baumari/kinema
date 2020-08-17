#ifndef K_THEODATA_HH
#define K_THEODATA_HH

#include <vector>
#include <fstream>
#include <KExpdata.hh>

// KTheodata can make spline function of stored data
// 

class KTheodata{
private:
  std::ifstream ifs;
public:
  typedef double (KTheodata::*SPLINE)(double *, double *);    
  std::vector<double> fx, fy;
  // corrected data for fitting  
  std::vector<double> fx_correct, fy_correct;
  SPLINE fspline;

public:  
  KTheodata();
  KTheodata(std::string& filename);
  KTheodata(char *filename);    
  ~KTheodata();
  void Open(std::string &filename);
  void Open(char *filename);
  void Print() const;
  // scale fy, fy_correct by factor
  void Scale(double factor); 
  void PrintCorrect() const;
  inline int GetN() const {return (int)fx.size();}
  inline int GetNCorrect() const {return (int)fx_correct.size();}
  double GetfxMin() const;
  double GetfxMax() const;
  double GetfxCorrectedMin() const;
  double GetfxCorrectedMax() const;  
  SPLINE GetSpline();
  double MakeSpline(double *, double *);
  double MakeSplineCorrect(double *, double *);  
  // correction by experimental data, especially for experimental resolution  
  void Correction(const KExpdataCS &);
  // return a function fx->fy, par is scale factor
  double Getf(double *x, double *par) const;
  // return a function fx_correct->fy_correct, par is scale factor
  double GetfCorrected(double *x, double *par) const;

private:
  // linear interpolation
  double Interpolate(double x);
  // return index for val (some TOL accepted)
  std::size_t FindIndex(const std::vector<double>& v, double val) const;
};

#endif
