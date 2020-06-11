#ifndef K_COLLISION_HH
#define K_COLLISION_HH

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
  void SetParticle(KParticle p1, KParticle p2, KParticle p3, KParticle p4);
  void ResultDump();
  void Init();
  inline double GetE3(int iParticle = 0){return m_E3[iParticle].back();}
  inline std::vector<double> GetE3Vec(int iParticle = 0){return m_E3[iParticle];}
  inline double GetE4(int iParticle = 0){return m_E4[iParticle].back();}
  inline std::vector<double> GetE4Vec(int iParticle = 0){return m_E4[iParticle];}  
  inline double GetT3(int iParticle = 0){return m_T3[iParticle].back();}
  inline std::vector<double> GetT3Vec(int iParticle = 0){return m_T3[iParticle];}  
  inline double GetT4(int iParticle = 0){return m_T4[iParticle].back();}
  inline std::vector<double> GetT4Vec(int iParticle = 0){return m_T4[iParticle];}  
  inline double GetTheta3CM(int iParticle = 0){return m_Theta3CM[iParticle].back();}
  inline std::vector<double> GetTheta3CMVec(int iParticle = 0){return m_Theta3CM[iParticle];}  
  inline double GetTheta4CM(int iParticle = 0){return m_Theta4CM[iParticle].back();}
  inline std::vector<double> GetTheta4CMVec(int iParticle = 0){return m_Theta4CM[iParticle];}    
  inline double GetTheta4(int iParticle = 0){return m_Theta4[iParticle].back();}
  inline std::vector<double> GetTheta4Vec(int iParticle = 0){return m_Theta4[iParticle];}
  inline std::vector<double> GetTheta3Vec(){return m_Theta3;}
  inline double GetGfactor(int iParticle = 0){return m_LabToCM[iParticle].back();}
  inline std::vector<double> GetGfacVec(int iParticle = 0){return m_LabToCM[iParticle];}  
  void Show();
  int GetParticleNum();
  inline KParticle GetP3(){return m_Finp3;}  
  inline KParticle GetP4(int iParticle = 0){return m_Finp4.at(iParticle);}
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
  void CheckNan();
};

#endif
