#ifndef K_EXPDATA_HH
#define K_EXPDATA_HH

#include <string>
#include <vector>

// normal expdata class
class KExpdata{
public:
  std::vector<double> fx, fy, fx_err, fy_err;

public:
  KExpdata();
  KExpdata(const char *filename);
  ~KExpdata() {}

public:
  void Print();
  void Scale(double factor); // scale experimental value by factor
  std::vector<double> GetX() {return fx;}
  std::vector<double> GetY() {return fy;}
  std::vector<double> GetXErr() {return fx_err;}
  std::vector<double> GetYErr() {return fy_err;}
  inline int GetN() const {return (int)fx.size();}
};

  // for cross section containing angular width
class KExpdataCS : public KExpdata{
public:
  std::vector<double> fx_width;

public:
  KExpdataCS();
  KExpdataCS(const char *filename);    
  ~KExpdataCS() {}
  void Print();
  std::vector<double> GetXWidth() {return fx_width;}

  //  ClassDef(KExpdata,1);
};

#endif
