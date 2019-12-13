#include <KFrame.hh>
#include <cmath>
#include <cstdlib>
#include <iostream>

KFrame::KFrame(KParticle &p1, KParticle &p2)
{
  SetParticle(p1, p2);
}

void KFrame::SetParticle(KParticle &p1, KParticle &p2)
{
  m_p[_P1] = p1; m_p[_P2] = p2;
  m_Beta.Set((m_p[_P1].P().P() + m_p[_P2].P().P())/(m_p[_P1].E() + m_p[_P2].E()));
  m_Lorentzfac = 1./sqrt(1-pow(m_Beta.Norm(), 2));
}

KParticle& KFrame::GetCMParticle(int No)
{
  if(No > 1){
    std::cerr << "KFrame::GetCMParticle: Particle index should be 0 or 1." << std::endl;
    std::exit(EXIT_FAILURE);
  }
  return m_pCM[No];
}
