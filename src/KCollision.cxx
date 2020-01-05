#include <KCollision.hh>
#include <cstdio>
#include <cstdlib>
#include <KError.hh>
#include <K4Momentum.hh>
#include <cmath>
#include <iostream>

KCollision::KCollision() {Initialize();}

KCollision::KCollision(KParticle& p1, KParticle& p2,
		       KParticle& p3, KParticle& p4)
{
  SetParticle(p1, p2, p3, p4);
  Initialize();
}

KCollision::KCollision(KParticle* p1, KParticle* p2,
		       KParticle* p3, KParticle* p4)
{
  SetParticle(p1, p2, p3, p4);
  Initialize();
}

KCollision::KCollision(KFrame& init, KFrame& fin)
{
  SetInitFrame(init);
  SetFinFrame(fin);
  Initialize();
}

KCollision::KCollision(KFrame* init, KFrame* fin)
{
  SetInitFrame(init);
  SetFinFrame(fin);
  Initialize();
}

void KCollision::Initialize()
{
  for(int iFrame = 0; iFrame != FRAME_SIZE; ++iFrame)
    m_Frame[iFrame] = NULL;
  for(int iParticle = 0; iParticle != PARTICLE_NUM_SIZE; ++iParticle){
    m_pLab[iParticle] = NULL;
    m_pCM[iParticle] = NULL; 
  }    
}

void KCollision::SetParticle(KParticle& p1, KParticle& p2,
			     KParticle& p3, KParticle& p4)
{
  SetInitParticle(p1, p2);
  SetFinParticle(p3, p4);  
}

void KCollision::SetParticle(KParticle* p1, KParticle* p2,
			     KParticle* p3, KParticle* p4)
{
  SetInitParticle(p1, p2);
  SetFinParticle(p3, p4);  
}

void KCollision::SetFrame(KFrame& init, KFrame& fin)
{
  SetInitFrame(init);
  SetFinFrame(fin);  
}

void KCollision::SetFrame(KFrame* init, KFrame* fin)
{
  SetInitFrame(init);
  SetFinFrame(fin);  
}

void KCollision::SetInitParticle(KParticle& p1, KParticle& p2)
{
  m_pLab[_P1] = &p1; m_pLab[_P2] = &p2;
  if(m_Frame[_INIT] != NULL){
    std::cout << "Initial particles are already defined??" << std::endl;
    std::cout
      << "Existing initial information will be replaced.." << std::endl;
    m_Frame[_INIT]->SetParticle(m_pLab[_P1], m_pLab[_P2]);
  }
}

void KCollision::SetInitParticle(KParticle* p1, KParticle* p2)
{
  m_pLab[_P1] = p1; m_pLab[_P2] = p2;
  if(m_Frame[_INIT] != NULL){
    std::cout << "Initial particles are already defined??" << std::endl;
    std::cout
      << "Existing initial information will be replaced.." << std::endl;
    m_Frame[_INIT]->SetParticle(m_pLab[_P1], m_pLab[_P2]);
  }
}

void KCollision::SetFinParticle(KParticle& p3, KParticle& p4)
{
  m_pLab[_P3] = &p3; m_pLab[_P4] = &p4;
  if(m_Frame[_FIN] != NULL){
    std::cout << "Initial particles are already defined??" << std::endl;
    std::cout
      << "Existing initial information will be replaced.." << std::endl;
    m_Frame[_FIN]->SetParticle(m_pLab[_P3], m_pLab[_P4]);
  }
}

void KCollision::SetFinParticle(KParticle *p3, KParticle *p4)
{
  m_pLab[_P3] = p3; m_pLab[_P4] = p4;
  if(m_Frame[_FIN] != NULL){
    std::cout << "Initial particles are already defined??" << std::endl;
    std::cout
      << "Existing initial information will be replaced.." << std::endl;
    m_Frame[_FIN]->SetParticle(m_pLab[_P3], m_pLab[_P4]);
  }
}

void KCollision::SetInitFrame(KFrame &init)
{
  m_Frame[_INIT] = &init;
}

void KCollision::SetInitFrame(KFrame *init)
{
  m_Frame[_INIT] = init;
}

void KCollision::SetFinFrame(KFrame &fin)
{
  m_Frame[_FIN] = &fin;
}

void KCollision::SetFinFrame(KFrame *fin)
{
  m_Frame[_FIN] = fin;
}

void KCollision::ResultDump()
{
  m_ResultBuff = stdout;
  ResultDumpCore();
}

void KCollision::ResultDump(FILE *fp)
{
  m_ResultBuff = fp;
  ResultDumpCore();
}

void KCollision::ResultDump(const char* FileName)
{
  m_ResultBuff = fopen(FileName, "w");
  if(m_ResultBuff == NULL){
    std::cerr << "Output file for calculated result cannot be opend!!"
	      << std::endl;
    std::cerr << "Results will be displayed instead." << std::endl;
    m_ResultBuff = stdout;
  }
  ResultDumpCore();
  if(m_ResultBuff != stdout) fclose(m_ResultBuff);
}

void KCollision::ResultDumpCore()
{
  fprintf(m_ResultBuff,
	  "#Theta3    #Theta4        #E3        #E4\n");
  for(std::size_t iResult = 0; iResult != m_Theta3.size(); ++iResult){
    fprintf(m_ResultBuff,
	    "%7.4lf     %7.4lf     %7.4lf     %7.4lf\n",
	    m_Theta3[iResult], m_Theta4[iResult], m_E3[iResult], m_E4[iResult]);
  }
}
//void KCollision::SetScattAngle(double angle)
//{
////  ClearAng();
////  m_AngRange[MIN] = angle;
////  m_AngRange[MAX] = angle;
////  m_ScattAng = angle;
//}
//
//void KCollision::SetScattAngle(double minang,
//			     double maxang, double delta)
//{
////  ClearAng();
////  m_AngRange[MIN]=minang; m_AngRange[MAX]=maxang;
////  m_AngRange[DELTA]=delta;
////  m_ScattAng=m_AngRange[MIN];
//}
//
//void KCollision::SetRecAngle(double angle)
//{
////  ClearAng();
////  m_AngRange[MIN] = angle;
////  m_AngRange[MAX] = angle;
////  m_RecAng = angle;
//}
//
//void KCollision::SetRecAngle(double minang,
//			   double maxang, double delta)
//{
////  ClearAng();
////  m_AngRange[MIN]=minang; m_AngRange[MAX]=maxang;
////  m_AngRange[DELTA]=delta;
////  m_RecAng=m_AngRange[MIN];  
//}
//
//void KCollision::ClearAngle()
//{
////  m_ScattAng=0;m_RecAng=0;
////  for(int i=0;i<ANG_RANGE_SIZE;i++) m_AngRange[i]=0;
//}
//
//void KCollision::Scatt()
//{
//}
//
//void KCollision::SetMandelstam()
//{
//  m_Mandelstam[S] = (m_pCM[P1].P()+m_pCM[P2].P()).Invaliant();
//  m_Mandelstam[T] = (m_pCM[P1].P()-m_pCM[P3].P()).Invaliant();
//  m_Mandelstam[U] = (m_pCM[P1].P()-m_pCM[P4].P()).Invaliant();  
//}
//
//void KCollision::SetMandelstamS()
//{
//  //  m_Mandelstam[S] = (m_pCM[P1].P()+m_pCM[P2].P()).Invaliant();  
//}
//
//void KCollision::SetMandelstamT()
//{
//  //  m_Mandelstam[T] = (m_pCM[P1].P()-m_pCM[P3].P()).Invaliant();  
//}
//
//void KCollision::SetMandelstamU()
//{
//  //  m_Mandelstam[U] = (m_pCM[P1].P()-m_pCM[P4].P()).Invaliant();    
//}
//
//void KCollision::SetLorentzFac()
//{
//  //  double Mass_tot = sqrt((m_pLab[P1]->P()+m_pLab[P2]->P()).Invaliant());
//  //  m_LorentzFac = (pow(Mass_tot,2)+pow(m_pLab[P2]->Mass(),2)
//  //		  -pow(m_pLab[P1]->Mass(),2))/2./Mass_tot/m_pLab[P2]->Mass();
//}
//
//K4Momentum KCollision::GetMomentumLab(int NofP)
//{
////  if(NofP >= PARTICLE_NUM_SIZE){
////    std::cerr << "Cannot access to Momentum indexed by " << NofP << "!!"
////	      << std::endl;
////    std::cerr << "Index must be less than 4!!" << std::endl;
////    m_errno = KError::INVALID_ARGUMENT;
////    std::exit(EXIT_FAILURE);
////  }
////  return m_pLab[NofP]->P();
//}
//
//K4Momentum KCollision::GetMomentumCM(int NofP)
//{
////  if(NofP >= PARTICLE_NUM_SIZE){
////    std::cerr << "Cannot access to Momentum indexed by " << NofP << "!!"
////	      << std::endl;
////    std::cerr << "Index must be less than 4!!" << std::endl;
////    m_errno = KError::INVALID_ARGUMENT;
////    std::exit(EXIT_FAILURE);
////  }
////  return m_pCM[NofP].P();
//}
