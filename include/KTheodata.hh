#ifndef K_THEODATA_HH
#define K_THEODATA_HH

#include <string>
#include <vector>
#include <KExpdata.hh>
#include <utility>

class KTheodata{
public:
  typedef double (KTheodata::*SPLINE)(double *, double *);    
  std::vector<double> fx, fy;
  // corrected data for fitting  
  std::vector<double> fx_correct, fy_correct;
  SPLINE fspline;

public:  
  KTheodata() : fspline(nullptr) {}
  KTheodata(const char *filename) : fspline(nullptr) {Open(filename);}
  KTheodata(const KTheodata &obj) :
    fx(obj.fx),
    fy(obj.fy),
    fx_correct(obj.fx_correct),
    fy_correct(obj.fy_correct) {}
  KTheodata(int nData); // only the number of data will be set
    
    
  ~KTheodata() {}
  void Open(const char *filename);
  void Print() const;
  // scale fy, fy_correct by factor
  void Scale(double factor); 
  void PrintCorrect() const;
  inline int GetN() const {return (int)fx.size();}
  inline int GetNCorrect() const {return (int)fx_correct.size();}
  std::size_t SetDataX(int nData, double *xdata);// return number of data points
  std::size_t SetDataY(int nData, double *ydata);
  std::size_t SetDataX(const std::vector<double>& xdata); 
  std::size_t SetDataY(const std::vector<double>& ydata);
  std::size_t SetDataXCorrect(int nData, double *xdata);
  std::size_t SetDataYCorrect(int nData, double *ydata);
  std::size_t SetDataXCorrect(const std::vector<double>& xdata); 
  std::size_t SetDataYCorrect(const std::vector<double>& ydata);    
  double GetfxMin() const;
  double GetfxMax() const;
  double GetfxCorrectMin() const;
  double GetfxCorrectMax() const;  
  SPLINE GetSpline();
  double MakeSpline(double *, double *);
  double MakeSplineCorrect(double *, double *);  
  // correction by experimental data, especially for experimental resolution  
  void Correction(const KExpdataCS &);
  // return a function fx->fy, par is scale factor
  double Getf(double *x, double *par) const;
  // return a function fx_correct->fy_correct, par is scale factor
  double GetfCorrected(double *x, double *par) const;
  void Clear(); // clear all stored data
  inline std::vector<double> GetX() {return fx;}
  inline std::vector<double> GetY() {return fy;}
  inline std::vector<double> GetXCorrect() {return fx_correct;}
  inline std::vector<double> GetYCorrect() {return fy_correct;}    
  
  KTheodata operator+(const KTheodata&) const;
  KTheodata& operator+=(const KTheodata&);

private:
  // linear interpolation
  double Interpolate(double x);
  // return index for val (some TOL accepted)
  std::size_t FindIndex(const std::vector<double>& v, double val) const;
};

#endif
