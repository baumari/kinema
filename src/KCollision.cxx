#include <KCollision.hh>
#include <cstdio>
#include <cstdlib>
#include <KError.hh>
#include <K4Momentum.hh>
#include <cmath>

KCollision::KCollision(KParticle& p1, KParticle& p2,
		       KParticle& p3, KParticle& p4)
{
  SetParticle(p1, p2, p3, p4);
}

void KCollision::SetParticle(KParticle& p1, KParticle& p2,
				    KParticle& p3, KParticle& p4)
{
  m_pLab[P1] = &p1; m_pLab[P2] = &p2; m_pLab[P3] = &p3; m_pLab[P4] = &p4;
  m_pCM[P1] = p1; m_pCM[P2] = p2; m_pCM[P3] = p3; m_pCM[P4] = p4;
  SetLorentzFac();  
  m_pCM[P1].P().BoostX(m_LorentzFac); m_pCM[P2].P().BoostX(m_LorentzFac);
  SetMandelstam();
}

void KCollision::SetScattAng(double minang,
			     double maxang, double delta)
{
  ClearAng();
  m_AngRange[MIN]=minang; m_AngRange[MAX]=maxang;
  m_AngRange[DELTA]=delta;
  m_ScattAng=m_AngRange[MIN];
}

void KCollision::SetRecAng(double minang,
			   double maxang, double delta)
{
  ClearAng();
  m_AngRange[MIN]=minang; m_AngRange[MAX]=maxang;
  m_AngRange[DELTA]=delta;
  m_RecAng=m_AngRange[MIN];  
}

void KCollision::ClearAng()
{
  m_ScattAng=0;m_RecAng=0;
  for(int i=0;i<ANG_RANGE_SIZE;i++) m_AngRange[i]=0;
}

void KCollision::Scatt()
{
}

void KCollision::SetMandelstam()
{
  K4Momentum s = m_pCM[P1].P()+m_pCM[P2].P();
  K4Momentum t = m_pCM[P1].P()-m_pCM[P3].P();
  K4Momentum u = m_pCM[P1].P()-m_pCM[P4].P();
  m_Mandelstam[S] = s.Invaliant();
  m_Mandelstam[T] = t.Invaliant();
  m_Mandelstam[U] = u.Invaliant();  
}

void KCollision::SetLorentzFac()
{
  double Mass_tot = m_pLab[0]->Mass() + m_pLab[1]->Mass();
  m_LorentzFac = (pow(Mass_tot,2)+pow(m_pLab[1]->Mass(),2)
		  -pow(m_pLab[0]->Mass(),2))/2./Mass_tot/m_pLab[1]->Mass();
}
