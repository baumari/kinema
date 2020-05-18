#ifndef _KEXPDATA_HH
#define _KEXPDATA_HH

#include <vector>
#include <fstream>

// normal expdata class
class KExpdata{
private:
  std::ifstream ifs;
public:
  std::vector<double> fx, fy, fx_err, fy_err;

public:
  KExpdata();
  KExpdata(std::string& filename);
  KExpdata(char *filename);    
  ~KExpdata();
  void Print();
  inline int GetN() const {return (int)fx.size();}
};

  // for cross section containing angular width
class KExpdataCS : public KExpdata{
private:
  std::ifstream ifs;
public:
  std::vector<double> fx_width;

public:
  KExpdataCS();
  KExpdataCS(std::string& filename);
  KExpdataCS(char *filename);    
  ~KExpdataCS();
  void Print();
};

#endif
