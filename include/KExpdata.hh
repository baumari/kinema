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
  KExpdata(std::string& filename);
  KExpdata(char *filename);
  ~KExpdata() {}

public:
  void Print();
  void Scale(double factor); // scale experimental value by factor
  inline int GetN() const {return (int)fx.size();}
};

  // for cross section containing angular width
class KExpdataCS : public KExpdata{
public:
  std::vector<double> fx_width;

public:
  KExpdataCS();
  KExpdataCS(std::string& filename);
  KExpdataCS(char *filename);    
  ~KExpdataCS() {}
  void Print();
};

#endif
