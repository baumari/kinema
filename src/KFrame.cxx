#include <KFrame.hh>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <KUtil.hh>
#include <K4Momentum.hh>

KFrame::KFrame(KParticle &p1, KParticle &p2)
{
  SetParticle(p1, p2);
}

KFrame::KFrame(KParticle *p1, KParticle *p2)
{
  SetParticle(p1, p2);
}

void KFrame::SetParticle(KParticle &p1, KParticle &p2)
{
  m_p[_P1] = &p1; m_p[_P2] = &p2;
}

void KFrame::SetParticle(KParticle *p1, KParticle *p2)
{
  m_p[_P1] = p1; m_p[_P2] = p2;
}

void KFrame::CM()
{
  K4Momentum Momentum[SIZE];
  Momentum[_P1] = m_p[_P1]->P(); Momentum[_P2] = m_p[_P2]->P();
  K4Momentum TotalMomentum = Momentum[_P1] + Momentum[_P2];
  K3Vector Beta = (TotalMomentum.P()/(Momentum[_P1].E() + Momentum[_P2].E()));
  Boost(Beta);
  m_Beta = Beta;
}

void KFrame::Boost(K3Vector &beta)
{
  m_p[_P1]->Boost(beta);
  m_p[_P2]->Boost(beta);  
}

void KFrame::Show()
{
  std::cout << "P1: "; m_p[_P1]->Show();
  std::cout << "P2: "; m_p[_P2]->Show();  
}
