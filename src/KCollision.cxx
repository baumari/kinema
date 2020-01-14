#include <KCollision.hh>
#include <cstdio>
#include <cstdlib>
#include <KError.hh>
#include <K4Momentum.hh>
#include <cmath>
#include <iostream>

KCollision::KCollision() {Initialize();}

KCollision::KCollision(KParticle p1, KParticle p2,
		       KParticle* p3, KParticle* p4)
{
  Initialize();
  SetInitParticle(p1, p2);
  SetFinParticle(p3, p4);
}

void KCollision::Initialize()
{
  // All memeber varables will be flushed.
  for(int i = 0; i != (int)INIT_P_SIZE; ++i){
    m_Initp[i].Initialize();
  }
  m_Finp3.Initialize();
  m_Finp4.clear();
  m_ResultBuff = NULL;
  m_M3 = 0; m_M4 = 0;
  m_Theta3.clear();
  m_E3CM = 0; m_E4CM = 0;
  m_Beta3CM = 0; m_Beta4CM = 0;
  m_Theta3CM.clear(); m_Theta4CM.clear(); m_Theta4.clear();
  m_E3.clear(); m_E4.clear();
  m_Beta3.clear(); m_Beta4.clear();
  m_LabToCM.clear();  
  m_AngleRange[_MIN] = 0.;
  m_AngleRange[_MAX] = 180.;
  m_AngleRange[_DELTA] = 1.;
  m_CalcFlag = 0;
}

void KCollision::SetInitParticle(KParticle p1, KParticle p2)
{
  m_Initp[_P1] = p1; m_Initp[_P2] = p2;
}

void KCollision::SetFinParticle(KParticle *p3, KParticle *p4)
{
  p3 = &m_Finp3; p4 = &m_Finp4.at(0);
}

void KCollision::SetFinMass(KParticle& p3, KParticle& p4)
{
  m_M3 = p3.Mass(); m_M4 = p4.Mass();
}

//void KCollision::ResultDump()
//{
//  m_ResultBuff = stdout;
//  ResultDumpCore();
//}
//
//void KCollision::ResultDump(FILE *fp)
//{
//  m_ResultBuff = fp;
//  ResultDumpCore();
//}
//
//void KCollision::ResultDump(const char* FileName)
//{
//  m_ResultBuff = fopen(FileName, "w");
//  if(m_ResultBuff == NULL){
//    std::cerr << "Output file for calculated result cannot be opend!!"
//	      << std::endl;
//    std::cerr << "Results will be displayed instead." << std::endl;
//    m_ResultBuff = stdout;
//  }
//  ResultDumpCore();
//  if(m_ResultBuff != stdout) fclose(m_ResultBuff);
//}
//
//void KCollision::ResultDumpCore()
//{
//  fprintf(m_ResultBuff,
//	  "#E3        #Theta3    #Theta3_CM     #E4        #Theta4    #Theta4_CM\n");
//  for(std::size_t iResult = 0; iResult != m_Theta3.size(); ++iResult){
//    fprintf(m_ResultBuff,
//	    "%8.4lf   %8.4lf   %8.4lf   %8.4lf   %8.4lf   %8.4lf\n",
//	    m_E3[iResult], m_Theta3[iResult], m_Theta3_CM[iResult],
//	    m_E4[iResult], m_Theta4[iResult], m_Theta4_CM[iResult]);
//  }
//}

void KCollision::SetScattAngle(double angle)
{
  m_Theta3 = angle;
  m_CalcFlag = _SCATT;
}

void KCollision::SetScattAngle(double minang,
			     double maxang, double delta)
{
  m_AngleRange[_MIN]=minang; m_AngleRange[_MAX]=maxang;
  m_AngleRange[_DELTA]=delta;
  m_CalcFlag = _SCATT_DUMP;
}

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

int KCollision::Scatt()
{
  int NumOfParticles = 0;
  switch(m_CalcFlag){
  case _SCATT:
    NumOfParticles = ScattCore();
    break;
  case _SCATT_DUMP:
    NumOfParticles = ScattDumpCore();
    break;
  case _RECOIL:
    NumOfParticles = RecoilCore();
    break;
  case _RECOIL_DUMP:
    NumOfParticles = RecoilDumpCore();
    break;
  default:
    std::cerr << "Angle information is not given!!" << std::endl;
    break;    
  }
  return NumOfParticles;
}

void KCollision::GetELab()
{
  for(std::size_t idx = 0; idx != m_Theta3CM.size(); ++idx){
    m_Beta3.push_back(m_Beta3CM*
		      sin(KUtil::DegToRad(m_Theta3CM[idx]))/sin(KUtil::DegToRad(m_Theta3)));
    m_E3.push_back(m_M3/sqrt(1-pow(m_Beta3.at(idx),2.)));
  }
  for(std::size_t idx = 0; idx != m_Theta4CM.size(); ++idx){
    m_Beta4.push_back(m_Beta4CM*
		      sin(KUtil::DegToRad(m_Theta4CM[idx]))/sin(KUtil::DegToRad(m_Theta4[idx])));
    m_E4.push_back(m_M4/sqrt(1-pow(m_Beta4.at(idx),2.)));
  }  
}
int KCollision::ScattCore()
{
  KFrame InitialFrame(m_Initp[_P1], m_Initp[_P2]);
  InitialFrame.CM();
  K3Vector Beta_CM = InitialFrame.GetBeta();
  double E1 = m_Initp[_P1].E(); double E2 = m_Initp[_P2].E();
  m_E3CM = ((E1+E2)*(E1+E2)+m_M3*m_M3-m_M4*m_M4)/2./(E1+E2);
  m_E4CM = ((E1+E2)-((E1+E2)*(E1+E2)+m_M3*m_M3-m_M4*m_M4)/2./(E1+E2));
  std::cout << m_E3CM << " " << m_E4CM << std::endl;
  m_Beta3CM = sqrt(pow(m_E3CM,2.)-pow(m_M3,2.))/m_E3CM;
  m_Beta4CM = sqrt(pow(m_E4CM,2.)-pow(m_M4,2.))/m_E4CM;  
  GetCMAngle(Beta_CM.Norm()/m_Beta3CM);
  GetRecoilAngle(Beta_CM.Norm()/m_Beta4CM);
  GetELab();  
  // store calculated result
  StoreResult();
  return (int)m_Theta4.size();
}

void KCollision::StoreResult()
{
  KParticle Part4; // for result
  double E3, E4;
  K3Vector P3, P4;
  double P3Norm, P4Norm;
  for(std::size_t idx = 0; idx != m_E3.size(); ++idx){
    E3 = m_M3/sqrt(1-pow(m_Beta3[idx], 2.));
    E4 = m_M4/sqrt(1-pow(m_Beta4[idx], 2.));
    P3Norm = E3*m_Beta3[idx];
    P4Norm = E4*m_Beta4[idx];
    P3.Set(P3Norm, 0., 0.);
    P4.Set(P4Norm, 0., 0.);
    P3.RotateZ(m_Theta3);
    P4.RotateZ(-m_Theta4[idx]);
    m_Finp3.SetMomentumComponent(E3, P3);    
    Part4.SetMomentumComponent(E4, P4);
    m_Finp4.push_back(Part4);
  }  
}

int KCollision::ScattDumpCore()
{
  KFrame InitialFrame(m_Initp[_P1], m_Initp[_P2]);
  InitialFrame.CM();
  K3Vector Beta_CM = InitialFrame.GetBeta();
  double InitialTotalEnergy_CM = m_Initp[_P1].E() + m_Initp[_P2].E();
  double E3_CM =
    (pow(InitialTotalEnergy_CM, 2.) + pow(m_M3, 2.) - pow(m_M4, 2.))
    /2./InitialTotalEnergy_CM;
  double E4_CM = InitialTotalEnergy_CM - E3_CM;
  double P3Norm_CM = sqrt(pow(E3_CM, 2.) - pow(m_M3, 2.));
  double P4Norm_CM = P3Norm_CM;
  double ThetaLab; // rad
  KParticle Part3, Part4; 
  // preparation for calculation
  K3Vector P3_CM(P3Norm_CM, 0, 0);
  P3_CM.RotateZ(KUtil::DegToRad(m_AngleRange[_MIN] - m_AngleRange[_DELTA])); 

  // calculation at each scattering angle
  for(double iAngle = m_AngleRange[_MIN]; iAngle <= m_AngleRange[_MAX];
      iAngle += m_AngleRange[_DELTA]){
    // particle #3
    m_Theta3CM.push_back(iAngle);
    P3_CM.RotateZ(KUtil::DegToRad(m_AngleRange[_DELTA]));
    Part3.SetMomentumComponent(E3_CM, P3_CM);
    Part3.Boost(-Beta_CM);
    m_E3.push_back(Part3.T());
    ThetaLab = asin(P3Norm_CM/sqrt(pow(Part3.E(), 2) - pow(Part3.Mass(), 2))
		    * sin(KUtil::DegToRad(iAngle)));
    m_Theta3.push_back(KUtil::RadToDeg(ThetaLab));
    // particle #4
    m_Theta4CM.push_back(180. - iAngle);
    Part4.SetMomentumComponent(E4_CM, -P3_CM);
    Part4.Boost(-Beta_CM);
    m_E4.push_back(Part4.T());
    if(iAngle < KUtil::EPSILON){
      m_Theta4.push_back(90.);
    }else{
      ThetaLab = asin(P4Norm_CM/sqrt(pow(Part4.E(), 2) - pow(Part4.Mass(), 2))
		      * sin(KUtil::DegToRad(180. - iAngle)));    
      m_Theta4.push_back(KUtil::RadToDeg(ThetaLab));
    }
  }
  return 0;
}

int KCollision::RecoilCore()
{
  return 0;
}

int KCollision::RecoilDumpCore()
{
  return 0;
}

void KCollision::GetCMAngle(double rho)
{
  double CosCM[2];
  double ThetaLab = KUtil::DegToRad(m_Theta3);
  double FirstTerm = -rho*pow(sin(ThetaLab),2.);
  double Determinant = pow(rho,2.)*pow(sin(ThetaLab),4.) - pow(rho,2.)*pow(sin(ThetaLab),2.)
    + pow(cos(ThetaLab), 2.);
  if(Determinant < 0){
    std::cout << "Invalid kinematical condition." << std::endl;
    return ;
  }else if(Determinant < KUtil::EPSILON){
    CosCM[0] = FirstTerm;
    if(fabs(CosCM[0]) > 1){
      std::cout << "Invalid kinematical condition." << std::endl;
      return ;
    }
    std::cout << "Only 1 kinematical condition mathced." << std::endl;
    m_Theta3CM.push_back(KUtil::RadToDeg(acos(CosCM[0])));
    m_Theta4CM.push_back(180.-KUtil::RadToDeg(acos(CosCM[0])));    
    return ;
  }
  double SecondTerm = sqrt(Determinant);
  CosCM[0] = FirstTerm + SecondTerm;
  CosCM[1] = FirstTerm - SecondTerm;
  if(fabs(CosCM[0]) > 1 && fabs(CosCM[1]) > 1){
    std::cout << "Invalid kinematical condition." << std::endl;
    return ;    
  }else if(fabs(CosCM[0]) <= 1 && fabs(CosCM[1]) <= 1){
    m_Theta3CM.push_back(KUtil::RadToDeg(acos(CosCM[0])));
    m_Theta3CM.push_back(KUtil::RadToDeg(acos(CosCM[1])));
    m_Theta4CM.push_back(180. - KUtil::RadToDeg(acos(CosCM[0])));
    m_Theta4CM.push_back(180. - KUtil::RadToDeg(acos(CosCM[1])));        
  }else{
    std::cout << "Only 1 kinematical condition mathced." << std::endl;
    m_Theta3CM.push_back(CosCM[0] <= 1 ? KUtil::RadToDeg(acos(CosCM[0])) :
			 KUtil::RadToDeg(acos(CosCM[1])));
    m_Theta4CM.push_back(180. - m_Theta3CM[0]);
    return ;
  }
}

//void GetRecCMAngle(double rho)
//{
//}

void KCollision::GetRecoilAngle(double rho)
{
  std::vector<double>::iterator it = m_Theta4CM.begin();
  double Theta4CM;
  for(; it != m_Theta4CM.end(); ++it){
    Theta4CM = KUtil::DegToRad(*it);
    if(fabs(rho + cos(Theta4CM)) < KUtil::EPSILON){
      m_Theta4.push_back(90.);
    }else {
      m_Theta4.push_back(KUtil::RadToDeg(atan(sin(Theta4CM)/(rho + cos(Theta4CM)))));
    }
  }
  return;
}

