#ifndef _KCOLLISION_HH
#define _KCOLLISION_HH

#include <KParticle.hh>
#include <K4Momentum.hh>
#include <KFrame.hh>
#include <cstdio>
#include <vector>

// x-y plane only

class KCollision{
private:
  enum Initial_P {
    _P1,
    _P2,
	   
    INIT_P_SIZE,
  };

  enum Calc_Opt {_SCATT = 1, _SCATT_DUMP,  _RECOIL, _RECOIL_DUMP, CALC_OPT_SIZE};

private:
  /*** Initialize condition for calculation ***/
  KParticle m_Initp[INIT_P_SIZE]; // _P1, _P2
  double m_M3, m_M4;  
  FILE* m_ResultBuff;
  std::vector<double> m_Theta3;

  /*** Storage for calculated result ***/
  KParticle m_Finp3; // _P3
  std::vector<KParticle> m_Finp4; // P4 (0 or 1 or 2)
  double m_E3CM, m_E4CM;
  double m_Beta3CM, m_Beta4CM;
  /**** vector structure  ****/
  /* No.0, No.1, No.2 */
  /* idx,  idx,  idx  */
  /* idx,  idx,  idx  */
  /* idx,  idx,  idx  */
  /* idx,  idx,  idx  */
  /* idx,  idx,  idx  */
  /* idx,  idx,  idx  */
  /**** vector structure ****/
  std::vector<std::vector<double> > m_Theta3CM, m_Theta4CM, m_Theta4;
  std::vector<std::vector<double> > m_E3, m_E4; // total energy
  std::vector<std::vector<double> > m_T3, m_T4; // kinematic energy  
  std::vector<std::vector<double> > m_LabToCM; // conversion factor for cross section
  
//  std::vector<double> m_LabToCM; // conversion factor for cross section
  int m_CalcFlag;
  int m_DumpFlag;

public:
  KCollision();  
  KCollision(KParticle p1, KParticle p2,
	     std::string p3, std::string p4);
  KCollision(KParticle* p1, KParticle* p2, //P1, P2 are converted into object
	     std::string p3, std::string p4);
  KCollision(KParticle p1, KParticle p2,
	     KParticle p3, KParticle p4);
  KCollision(KParticle *p1, KParticle *p2,
	     KParticle p3, KParticle p4);
  ~KCollision() {}

public:
  void SetInitParticle(KParticle p1, KParticle p2);
  void SetInitParticle(KParticle* p1, KParticle* p2);//P1, P2 are converted into object
  void SetFinParticle(std::string p3, std::string p4);
  void SetFinParticle(KParticle p3, KParticle p4);  
  void ResultDump();
  void Init();
  int GetParticleNum();
  inline KParticle GetP3(){return m_Finp3;}  
  inline KParticle GetP4(int iParticle){return m_Finp4.at(iParticle);}
  void ResultDump(FILE *fp);  
  void ResultDump(const char* FileName);
  void SetScattAngle(double angle); // Lab angle
  void SetScattAngle(double minang, double maxang, double delta); // in CM frame
//  inline void SetRecAng(double angle){ClearAng();m_RecAng=angle;}
//  void SetRecAng(double minang, double maxang, double delta);
//  inline double GetScattAng() {return m_ScattAng;}
  int Scatt(); // return val is number of detecting particles__ 0, 1, 2
  void Clear(); // forget previous result of calcualtion  

private:
  void InitDump();
  void ClearAngle();
  void ResultDumpCore();
  int ScattCore();
  int ScattDumpCore();
  int RecoilCore();
  int RecoilDumpCore();
  void GetCMAngle(double, double); // (beta_CM, beta3_CM) // for m_Theta3CM, m_Theta4CM
  void GetRecoilAngle(double, double); // (beta_CM, beta4_CM) // for m_Theta4
  void Getfac(double); // (beta_CM) // conversion factor (cm = lab*fac)
  //  void GetRecCMAngle(double rho); // (beta_CM/beta4_CM) 
  void GetELab(double); //(beta_CM) for m_E3, m_E4
  void StoreResult();
};

#endif
