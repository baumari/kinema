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
  K4Momentum TotalMomentumLab = Momentum[_P1] + Momentum[_P2];
  m_Beta.Set(TotalMomentumLab.P()/(Momentum[_P1].E() + Momentum[_P2].E()));
  m_Lorentzfac = KUtil::BetaToGamma(m_Beta);
  /* If speed of CM frame is too small, no conversion is ocurred. */
  if(TotalMomentumLab.P().Norm() < KUtil::EPSILON) return ;
  double EnergyCM_P1 = m_Lorentzfac*(Momentum[_P1].E()
				     -m_Beta*Momentum[_P1].P());
  K3Vector pCM_P1; 
  pCM_P1 = -m_Beta*(m_Lorentzfac*Momentum[_P1].E()) + Momentum[_P1].P() +
    m_Beta*((m_Beta*Momentum[_P1].P())*((m_Lorentzfac-1)/pow(m_Beta.Norm(),2)));
  m_p[_P1]->SetEnergyDirection(EnergyCM_P1, pCM_P1);
  // Momentum of P1 in CM frame was obtained above.
  double EnergyCM_P2 =
    m_Lorentzfac*(Momentum[_P2].E()-m_Beta*Momentum[_P2].P());
  K3Vector pCM_P2(-pCM_P1);
  m_p[_P2]->SetEnergyDirection(EnergyCM_P2, pCM_P2);
  return ;
}

KParticle* KFrame::GetParticle(int No)
{
  if(No > 1){
    std::cerr << "KFrame::GetCMParticle: Particle index should be 0 or 1." << std::endl;
    std::exit(EXIT_FAILURE);
  }
  return m_p[No];
}

void KFrame::Show()
{
  std::cout << "P1: "; m_p[_P1]->Show();
  std::cout << "P2: "; m_p[_P2]->Show();  
}
