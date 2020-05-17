#ifndef _KEXPDATA_HH
#define _KEXPDATA_HH

#include <vector>

// normal expdata class
class KExpdata{
public:
  std::vector<double> fx, fy, fx_err, fy_err;

public:
  KExpdata();
  ~KExpdata();
  void Print();
  inline int GetN() const {return (int)fx.size();}
};

  // for cross section containing angular width
class KExpdataCS : public KExpdata{
public:
  std::vector<double> fx_width;

public:
  KExpdataCS();
  ~KExpdataCS();
  void Print();
};

#endif
