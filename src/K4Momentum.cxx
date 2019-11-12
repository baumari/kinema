#include <K4Momentum.hh>
#include <K3Vector.hh>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <KUtil.hh>

K4Momentum::K4Momentum(double energy, double px, double py, double pz)
{
  m_E = energy;
  m_P.Set(px, py, pz);  
}

void K4Momentum::Set(const double energy,
		     const double px, const double py, const double pz)
{
  m_E = energy;
  m_P.Set(px, py, pz);    
}

void K4Momentum::Set(const double energy, const K3Vector p)
{
  m_E = energy;
  m_P = p;
}

double K4Momentum::Invaliant() const
{
  return (pow(m_E,2)-pow(m_P.Norm(),2));
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
  fprintf(stdout, "P1: %lf, P2: %lf, P3:%lf, P4:%lf\n",
	  m_E, m_P.X(), m_P.Y(), m_P.Z());
}

void K4Momentum::BoostX(double gamma)
{
  double beta = KUtil::GammaToBeta(gamma);
  Set(m_E*gamma-beta*gamma*m_P.X(),
      -beta*gamma*m_E+gamma*m_P.X(), m_P.Y(), m_P.Z());
}

void K4Momentum::BoostY(double gamma)
{
  double beta = KUtil::GammaToBeta(gamma);
  Set(m_E*gamma-beta*gamma*m_P.Y(), m_P.X(),
      -beta*gamma*m_E+gamma*m_P.Y(), m_P.Z());
}

void K4Momentum::BoostZ(double gamma)
{
  double beta = KUtil::GammaToBeta(gamma);
  Set(m_E*gamma-beta*gamma*m_P.Z(), m_P.X(), m_P.Y(),
      -beta*gamma*m_E+gamma*m_P.Z());  
}
