#ifndef K_THEODATA_HH
#define K_THEODATA_HH

#include <vector>
#include <fstream>

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
  void Print();
  void Scale(double factor); // scale fy, fy_correct by factor
  void PrintCorrect();
  inline int GetN() const {return (int)fx.size();}
  inline int GetNCorrect() const {return (int)fx_correct.size();}
  double GetfxMin();
  double GetfxMax();
  SPLINE GetSpline();
  double MakeSpline(double *, double *);  // do not call this function out of class
};

#endif
