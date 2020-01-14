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

  enum Final_P {
    _P3,
    _P4,

    FIN_P_SIZE,
  };

  enum Ang_Range {
    _MIN,
    _MAX,
    _DELTA,
		  
    ANG_RANGE_SIZE,
  };

  enum Calc_Opt {_SCATT = 1, _SCATT_DUMP,  _RECOIL, _RECOIL_DUMP, CALC_OPT_SIZE};

private:
  KParticle m_Initp[INIT_P_SIZE]; // _P1, _P2
  KParticle m_Finp3; // _P3
  std::vector<KParticle> m_Finp4;
  FILE* m_ResultBuff;
  double m_M3, m_M4;
  std::vector<double> m_Theta3;
  double m_E3CM, m_E4CM;
  double m_Beta3CM, m_Beta4CM;
  std::vector<std::vector<double> > m_Theta3CM, m_Theta4CM, m_Theta4;
  std::vector<std::vector<double> > m_E3, m_E4; // kinematic energy
  std::vector<std::vector<double> > m_Beta3, m_Beta4; 
  std::vector<std::vector<double> > m_LabToCM; // conversion factor for cross section
  
//  std::vector<double> m_LabToCM; // conversion factor for cross section
  double m_AngleRange[ANG_RANGE_SIZE];
  int m_CalcFlag;

public:
  KCollision();  
  KCollision(KParticle p1, KParticle p2,
	     KParticle* p3, KParticle* p4);
  ~KCollision() {}

public:
  void SetInitParticle(KParticle p1, KParticle p2);
  void SetFinParticle(KParticle* p3, KParticle* p4);
  void SetFinMass(KParticle& p3, KParticle& p4); // get final mass
  void ResultDump();
  inline void Init() {Initialize();}
//  void ResultDump(FILE *fp);  
//  void ResultDump(const char* FileName);
  void SetScattAngle(double angle); // Lab angle
  void SetScattAngle(double minang, double maxang, double delta); // in CM frame
//  inline void SetRecAng(double angle){ClearAng();m_RecAng=angle;}
//  void SetRecAng(double minang, double maxang, double delta);
//  inline double GetScattAng() {return m_ScattAng;}
  int Scatt(); // return val is number of detecting particles__ 0, 1, 2
  
private:
  void Initialize();
  void ResultDumpCore();
  int ScattCore();
  int ScattDumpCore();
  int RecoilCore();
  int RecoilDumpCore();
  void GetCMAngle(double rho); // (beta_CM/beta3_CM) // for m_Theta3CM, m_Theta4CM
  void GetRecoilAngle(double rho); // (beta_CM/beta4_CM) // for m_Theta4
  //  void GetRecCMAngle(double rho); // (beta_CM/beta4_CM) 
  void GetELab(); // for m_E3, m_E4
  void StoreResult();
};

#endif
