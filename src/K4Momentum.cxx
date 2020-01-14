#include <K4Momentum.hh>
#include <K3Vector.hh>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <KUtil.hh>
#include <iostream>

K4Momentum::K4Momentum(double energy, double px, double py, double pz)
{
  m_E = energy;
  m_P.Set(px, py, pz);
  CheckOffShell();
}

void K4Momentum::Set(const double energy,
		     const double px, const double py, const double pz)
{
  m_E = energy;
  m_P.Set(px, py, pz);
  CheckOffShell();
}

void K4Momentum::Set(const double energy, const K3Vector p)
{
  m_E = energy;
  m_P = p;
  CheckOffShell();
}

double K4Momentum::Invaliant() const
{
  return (sqrt(pow(m_E,2)-pow(m_P.Norm(),2)));
}

K4Momentum& K4Momentum::operator=(const K4Momentum& p)
{
  m_E = p.E();
  m_P = p.P();
  return *this;
}

K4Momentum K4Momentum::operator+(const K4Momentum& p) const
{
  K4Momentum tmp;
  tmp.Set(m_E+p.E(), m_P+p.P());
  return tmp;
}

K4Momentum K4Momentum::operator-(const K4Momentum& p) const
{
  K4Momentum tmp;
  tmp.Set(m_E-p.E(), m_P-p.P());
  return tmp;
}

double K4Momentum::operator*(const K4Momentum& p) const
{
  return m_E*p.E()-m_P*p.P();
}

K4Momentum K4Momentum::operator-() const
{
  K4Momentum tmp;
  tmp.Set(-m_E, -m_P);
  return tmp;
}

void K4Momentum::Show() const 
{
  printf("(%lf %lf %lf %lf)\n",
	 m_E, m_P.X(), m_P.Y(), m_P.Z());
}

void K4Momentum::Boost(K3Vector beta)
{
  double gamma = KUtil::BetaToGamma(beta);
  double E = gamma*(m_E - beta*m_P);
  K3Vector P = -beta*gamma*m_E + m_P + beta*(gamma - 1)/(pow(beta.Norm(), 2))*(beta*m_P);
  Set(E, P);
}

void K4Momentum::Boost(double x, double y, double z)
{
  K3Vector BetaVec(x, y, z);
  Boost(BetaVec);
}

void K4Momentum::BoostX(double beta)
{
  K3Vector BetaVec(beta, 0, 0);
  Boost(BetaVec);  
}

void K4Momentum::BoostY(double beta)
{
  K3Vector BetaVec(0, beta, 0);
  Boost(BetaVec);  
}

void K4Momentum::BoostZ(double beta)
{
  K3Vector BetaVec(0, 0, beta);
  Boost(BetaVec);  
}

void K4Momentum::CheckOffShell()
{
  if(pow(m_E,2) - pow(m_P.Norm(), 2) < 0){
    std::cout << "This momentum is off-shell condition!!!: ";
    Show();
  }  
}
