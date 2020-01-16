#include <KCollision.hh>
#include <cstdio>
#include <cstdlib>
#include <KError.hh>
#include <K4Momentum.hh>
#include <cmath>
#include <iostream>

KCollision::KCollision() {Init();}

KCollision::KCollision(KParticle p1, KParticle p2,
		       std::string p3, std::string p4)
{
  Init();
  SetInitParticle(p1, p2);
  SetFinParticle(p3, p4);
}

KCollision::KCollision(KParticle* p1, KParticle* p2,
		       std::string p3, std::string p4)
{
  Init();
  SetInitParticle(p1, p2);
  SetFinParticle(p3, p4);
}

void KCollision::Init()
{
  // All memeber varables will be flushed.
  m_Initp[_P1].Init();  m_Initp[_P2].Init();
  m_Finp3.Init();
  m_Finp4.clear();
  m_ResultBuff = NULL;
  m_M3 = 0; m_M4 = 0;
  ClearAngle();
  m_E3CM = 0; m_E4CM = 0;
  m_Beta3CM = 0; m_Beta4CM = 0;
  m_E3.clear(); m_E4.clear();
  m_Beta3.clear(); m_Beta4.clear();
  m_LabToCM.clear();  
  m_CalcFlag = 0;
  m_DumpFlag = 0;
}

void KCollision::SetInitParticle(KParticle p1, KParticle p2)
{
  m_Initp[_P1] = p1; m_Initp[_P2] = p2;
}

void KCollision::SetInitParticle(KParticle* p1, KParticle* p2)
{
  m_Initp[_P1] = *p1; m_Initp[_P2] = *p2;
}

void KCollision::SetFinParticle(std::string p3, std::string p4)
{
  KParticle P3(p3);  KParticle P4(p4); // only for tmp
  m_M3 = P3.Mass(); m_M4 = P4.Mass();
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
    std::cerr << "Output file for calculated result cannot be opened!!"
	      << std::endl;
    std::cerr << "Results will be displayed instead." << std::endl;
    m_ResultBuff = stdout;
  }
  ResultDumpCore();
  if(m_ResultBuff != stdout) fclose(m_ResultBuff);
}

void KCollision::ResultDumpCore()
{
  if(!m_DumpFlag){
    std::cerr << "Results will not be dumped. You must any ***Dump method before." << std::endl;
    return ;
  }
  fprintf(m_ResultBuff,
	  "#E3        #Theta3    #Theta3_CM     #E4        #Theta4    #Theta4_CM\n");
  for(std::size_t iResult = 0; iResult != m_Theta3.size(); ++iResult){
    fprintf(m_ResultBuff,
	    "%8.4lf   %8.4lf   %8.4lf   %8.4lf   %8.4lf   %8.4lf\n",
	    m_E3[0][iResult], m_Theta3[iResult], m_Theta3CM[0][iResult],
	    m_E4[0][iResult], m_Theta4[0][iResult], m_Theta4CM[0][iResult]);
  }
}

void KCollision::SetScattAngle(double angle)
{
  ClearAngle();
  m_Theta3.push_back(angle);
  m_CalcFlag = _SCATT;
}

void KCollision::SetScattAngle(double minang,
			     double maxang, double delta)
{
  ClearAngle();
  InitDump();
  for(double iang = minang; iang <= maxang; iang += delta){
    m_Theta3CM[0].push_back(iang);
  }
  m_CalcFlag = _SCATT_DUMP;
}

void KCollision::ClearAngle()
{
  m_Theta3.clear(); m_Theta4.clear();
  m_Theta3CM.clear(); m_Theta4CM.clear();  
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
    Clear();      
    break;
  case _SCATT_DUMP:
    NumOfParticles = ScattDumpCore();
    m_DumpFlag = 1;
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
  m_Beta3.resize(m_Theta3CM.size()); m_E3.resize(m_Theta3CM.size());
  m_Beta4.resize(m_Theta4CM.size()); m_E4.resize(m_Theta4CM.size());  
  for(std::size_t idxOuter = 0; idxOuter != m_Theta3CM.size(); ++idxOuter){
    for(std::size_t idxInner = 0; idxInner != m_Theta3CM[idxOuter].size(); ++idxInner){
      m_Beta3[idxOuter].push_back(m_Beta3CM*
				  sin(KUtil::DegToRad(m_Theta3CM[idxOuter][idxInner]))
				  /sin(KUtil::DegToRad(m_Theta3[idxInner])));
      m_E3[idxOuter].push_back(m_M3/sqrt(1-pow(m_Beta3.at(idxOuter).at(idxInner),2.)));    
    }
  }
  for(std::size_t idxOuter = 0; idxOuter != m_Theta4CM.size(); ++idxOuter){
    for(std::size_t idxInner = 0; idxInner != m_Theta4CM[idxOuter].size(); ++idxInner){
      m_Beta4[idxOuter].push_back(m_Beta4CM*
			sin(KUtil::DegToRad(m_Theta4CM[idxOuter][idxInner]))
				  /sin(KUtil::DegToRad(m_Theta4[idxOuter][idxInner])));
      m_E4[idxOuter].push_back(m_M4/sqrt(1-pow(m_Beta4.at(idxOuter).at(idxInner),2.)));
    }
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
  KParticle Part4;
  K3Vector P3(1, 0, 0);
  K3Vector P4(1, 0, 0);
  for(std::size_t idx = 0; idx != m_E3.size(); ++idx){    
    P3.RotateZ(m_Theta3[0]);
    P4.RotateZ(-m_Theta4[idx][0]);
    m_Finp3.SetMass(m_M3); Part4.SetMass(m_M4);
    m_Finp3.SetEnergyDirection(m_E3[idx][0], P3);
    Part4.SetEnergyDirection(m_E4[idx][0], P4);
    m_Finp4.push_back(Part4);
  }  
}

int KCollision::ScattDumpCore()
{
  KFrame InitialFrame(m_Initp[_P1], m_Initp[_P2]);
  InitialFrame.CM();
  K3Vector Beta_CM = InitialFrame.GetBeta();
  double InitialTotalEnergy_CM = m_Initp[_P1].E() + m_Initp[_P2].E();
  m_E3CM =
    (pow(InitialTotalEnergy_CM, 2.) + pow(m_M3, 2.) - pow(m_M4, 2.))
    /2./InitialTotalEnergy_CM;
  m_E4CM = InitialTotalEnergy_CM - m_E3CM;
  double ThetaLab; // rad
  KParticle Part3, Part4; 
  // preparation for calculation
  K3Vector P3_CM(1., 0., 0.);
  double delta = m_Theta3CM[0][1] - m_Theta3CM[0][0];
  P3_CM.RotateZ(KUtil::DegToRad(m_Theta3CM[0][0] - delta));  
// calculation at each scattering angle
  for(std::vector<double>::iterator it = m_Theta3CM[0].begin(); it != m_Theta3CM[0].end(); ++it){
    // particle #3
    P3_CM.RotateZ(KUtil::DegToRad(delta));
    Part3.SetMass(m_M3);
    Part3.SetEnergyDirection(m_E3CM, P3_CM);
    Part3.Boost(-Beta_CM); // into lab frame
    m_E3[0].push_back(Part3.T());
    ThetaLab = asin(sqrt(pow(m_E3CM, 2) - pow(m_M3, 2))/sqrt(pow(Part3.E(), 2) - pow(m_M3, 2))
		    * sin(KUtil::DegToRad(*it)));
    m_Theta3.push_back(KUtil::RadToDeg(ThetaLab));
    // particle #4
    m_Theta4CM[0].push_back(180. - *it);
    Part4.SetMass(m_M4);
    Part4.SetEnergyDirection(m_E4CM, -P3_CM);
    Part4.Boost(-Beta_CM);
    m_E4[0].push_back(Part4.T());
    if(*it < KUtil::EPSILON){
      m_Theta4[0].push_back(90.);
    }else{
      ThetaLab = asin(sqrt(pow(m_E4CM, 2) - pow(m_M4, 2))/sqrt(pow(Part4.E(), 2.)-pow(m_M4,2.))
		      * sin(KUtil::DegToRad(180. - *it)));    
      m_Theta4[0].push_back(KUtil::RadToDeg(ThetaLab));
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
  double ThetaLab; double FirstTerm; double Determinant;
  std::vector<double>::iterator it = m_Theta3.begin();
  for(; it != m_Theta3.end(); ++it){
    ThetaLab = KUtil::DegToRad(*it);
    FirstTerm = -rho*pow(sin(ThetaLab),2.);
    Determinant = pow(rho,2.)*pow(sin(ThetaLab),4.) - pow(rho,2.)*pow(sin(ThetaLab),2.)
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
      m_Theta3CM.resize(1); m_Theta4CM.resize(1);
      m_Theta3CM[0].push_back(KUtil::RadToDeg(acos(CosCM[0])));
      m_Theta4CM[0].push_back(180.-KUtil::RadToDeg(acos(CosCM[0])));    
      return ;
    }
    double SecondTerm = sqrt(Determinant);
    CosCM[0] = FirstTerm + SecondTerm;
    CosCM[1] = FirstTerm - SecondTerm;
    if(fabs(CosCM[0]) > 1 && fabs(CosCM[1]) > 1){
      std::cout << "Invalid kinematical condition." << std::endl;
      return ;    
    }else if(fabs(CosCM[0]) <= 1 && fabs(CosCM[1]) <= 1){
      m_Theta3CM.resize(2); m_Theta4CM.resize(2);
      m_Theta3CM[0].push_back(KUtil::RadToDeg(acos(CosCM[0])));
      m_Theta3CM[1].push_back(KUtil::RadToDeg(acos(CosCM[1])));
      m_Theta4CM[0].push_back(180. - KUtil::RadToDeg(acos(CosCM[0])));
      m_Theta4CM[1].push_back(180. - KUtil::RadToDeg(acos(CosCM[1])));        
    }else{
      std::cout << "Only 1 kinematical condition mathced." << std::endl;
      m_Theta3CM.resize(1); m_Theta4CM.resize(1);
      m_Theta3CM[0].push_back(CosCM[0] <= 1 ? KUtil::RadToDeg(acos(CosCM[0])) :
			   KUtil::RadToDeg(acos(CosCM[1])));
      m_Theta4CM[0].push_back(180. - m_Theta3CM[0].at(0));
      return ;
    }    
  }  
}

//void GetRecCMAngle(double rho)
//{
//}

void KCollision::GetRecoilAngle(double rho)
{
  m_Theta4.resize(m_Theta4CM.size());
  double Theta4CM;
  for(std::size_t idxOuter = 0; idxOuter != m_Theta4CM.size(); ++idxOuter){
    for(std::size_t idxInner = 0; idxInner != m_Theta4CM[idxOuter].size(); ++idxInner){
      Theta4CM = KUtil::DegToRad(m_Theta4CM[idxOuter][idxInner]);
      if(fabs(rho + cos(Theta4CM)) < KUtil::EPSILON){
	m_Theta4[idxOuter].push_back(90.);
      }else {
	m_Theta4[idxOuter].push_back(KUtil::RadToDeg(atan(sin(Theta4CM)/(rho + cos(Theta4CM)))));
      }
    }
  }
  return;
}

int KCollision::GetParticleNum()
{
  return (int)m_Theta4.size();
}

void KCollision::Clear()
{
  m_Finp3.Init();
  m_Finp4.clear();
  m_E3CM = 0; m_E4CM = 0;
  m_Beta3CM = 0; m_Beta4CM = 0;
  m_Theta3CM.clear(); m_Theta4CM.clear(); m_Theta4.clear();
  m_E3.clear(); m_E4.clear();
  m_Beta3.clear(); m_Beta4.clear();
  m_LabToCM.clear();
}

 void KCollision::InitDump()
 {
   Clear();
   m_Theta3CM.resize(1);
   m_Theta4CM.resize(1);
   m_Theta4.resize(1);
   m_E3.resize(1); m_E4.resize(1);
 }
