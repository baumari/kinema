#include <KCollision.hh>
#include <cstdio>
#include <cstdlib>
#include <KError.hh>
#include <K4Momentum.hh>
#include <cmath>
#include <iostream>

KCollision::KCollision(KParticle& p1, KParticle& p2,
		       KParticle& p3, KParticle& p4)
{
  SetParticle(p1, p2, p3, p4);
}

void KCollision::SetParticle(KParticle& p1, KParticle& p2,
			     KParticle& p3, KParticle& p4)
{
  SetInitParticle(p1, p2);
  SetFinParticle(p3, p4);  
}

void KCollision::SetInitParticle(KParticle& p1, KParticle& p2)
{
  m_pLab[P1] = &p1; m_pLab[P2] = &p2; m_pCM[P1] = p1; m_pCM[P2] = p2;
  SetLorentzFac();
  m_pCM[P1].BoostX(m_LorentzFac); m_pCM[P2].BoostX(m_LorentzFac);
  SetMandelstamS();
}

void KCollision::SetFinParticle(KParticle& p3, KParticle& p4)
{
  m_pLab[P3] = &p3; m_pLab[P4] = &p4;
}

void KCollision::SetScattAng(double angle)
{
  ClearAng();
  m_AngRange[MIN] = angle;
  m_AngRange[MAX] = angle;
  m_ScattAng = angle;
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
  m_Mandelstam[S] = (m_pCM[P1].P()+m_pCM[P2].P()).Invaliant();
  m_Mandelstam[T] = (m_pCM[P1].P()-m_pCM[P3].P()).Invaliant();
  m_Mandelstam[U] = (m_pCM[P1].P()-m_pCM[P4].P()).Invaliant();  
}

void KCollision::SetMandelstamS()
{
  m_Mandelstam[S] = (m_pCM[P1].P()+m_pCM[P2].P()).Invaliant();  
}

void KCollision::SetMandelstamT()
{
  m_Mandelstam[T] = (m_pCM[P1].P()-m_pCM[P3].P()).Invaliant();  
}

void KCollision::SetMandelstamU()
{
  m_Mandelstam[U] = (m_pCM[P1].P()-m_pCM[P4].P()).Invaliant();    
}

void KCollision::SetLorentzFac()
{
  double Mass_tot = sqrt((m_pLab[P1]->P()+m_pLab[P2]->P()).Invaliant());
  m_LorentzFac = (pow(Mass_tot,2)+pow(m_pLab[P2]->Mass(),2)
		  -pow(m_pLab[P1]->Mass(),2))/2./Mass_tot/m_pLab[P2]->Mass();
}

K4Momentum KCollision::GetMomentumLab(int NofP)
{
  if(NofP >= PARTICLE_NUM_SIZE){
    std::cerr << "Cannot access to Momentum indexed by " << NofP << "!!"
	      << std::endl;
    std::cerr << "Index must be less than 4!!" << std::endl;
    m_errno = KError::INVALID_ARGUMENT;
    std::exit(EXIT_FAILURE);
  }
  return m_pLab[NofP]->P();
}

K4Momentum KCollision::GetMomentumCM(int NofP)
{
  if(NofP >= PARTICLE_NUM_SIZE){
    std::cerr << "Cannot access to Momentum indexed by " << NofP << "!!"
	      << std::endl;
    std::cerr << "Index must be less than 4!!" << std::endl;
    m_errno = KError::INVALID_ARGUMENT;
    std::exit(EXIT_FAILURE);
  }
  return m_pCM[NofP].P();
}
