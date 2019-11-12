#ifndef _K4MOMENTUM_HH
#define _K4MOMENTUM_HH

#include <K3Vector.hh>

class K4Momentum
{
private:
  double m_E;
  K3Vector m_P;

public:
  K4Momentum() {}
  K4Momentum(double energy, double px, double py, double pz);
  ~K4Momentum() {}

public:
  inline double E() const {return m_E;}
  inline double Px() const {return m_P.X();}
  inline double Py() const {return m_P.Y();}
  inline double Pz() const {return m_P.Z();}
  inline K3Vector P() const {return m_P;}
  void Set(double energy, double px, double py, double pz);
  void Set(double energy, K3Vector p);  
  double Invaliant() const;
  K4Momentum& operator=(const K4Momentum& p);
  K4Momentum operator+(const K4Momentum& p) const;
  K4Momentum operator-(const K4Momentum& p) const;
  double operator*(const K4Momentum& p) const;
  inline K4Momentum operator+() const {return *this;}
  K4Momentum operator-() const;
  void Show() const;
  void BoostX(double gamma);
  void BoostY(double gamma);
  void BoostZ(double gamma);  
};

#endif
