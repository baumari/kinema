#include <FVector.hh>
#include <cmath>

FVector::FVector()
{
  for(int i=0;i<5;i++){
    m_p[i]=0;
  }
}

FVector::~FVector(){}

double FVector::mass()
{
  return m_p[4];
}

double FVector::e()
{
  return m_p[0];
}
double FVector::px()
{
  return m_p[1];
}
double FVector::py()
{
  return m_p[2];
}
double FVector::pz()
{
  return m_p[3];
}

double operator*(FVector v1, FVector v2)
{
  double val;
  val=v1.e()*v2.e()-v1.px()*v2.px()-v1.py()*v2.py()-v1.pz()*v2.pz();
  return val;
}
  
void FVector::setkine(double kine)
{
  m_p[0]=kine+m_p[4];
}

void FVector::setmass(double p4)
{
  m_p[4]=p4;
}

void FVector::inite(double mass, double kine)
{
  m_p[4]=mass;
  m_p[0]=mass+kine;
}

void FVector::update()
{
  double gamma=m_p[0]/m_p[4];
  double beta=sqrt(1-1/gamma/gamma);
  m_p[3]=m_p[4]*beta*gamma;
}
