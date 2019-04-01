#include <FVector.hh>

FVector::FVector()
{
  for(int i=0;i<4;i++){
    m_p[i]=0;
  }
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
  
