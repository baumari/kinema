#ifndef _KCOLLISION_HH
#define _KCOLLISION_HH

#include <KParticle.hh>
#include <K4Momentum.hh>
#include <KFrame.hh>

// x-y plane only

class KCollision{
private:
  enum Particle_Num {
    _P1,
    _P2,
    _P3,
    _P4,
	   
    PARTICLE_NUM_SIZE,
  };

  enum Ang_Range {
    _MIN,
    _MAX,
    _DELTA,
		  
    ANG_RANGE_SIZE,
  };

  enum Mandelstam {
    _S,
    _T,
    _U,

    MANDEL_SIZE,
  };

  enum Frame {
    _INIT,
    _FIN,

    FRAME_SIZE,
  };

private:
  KFrame *m_Frame[FRAME_SIZE];
  KParticle *m_pLab[PARTICLE_NUM_SIZE];
  KParticle *m_pCM[PARTICLE_NUM_SIZE];  
//  double m_ScattAng;
//  double m_RecAng;
//  double m_AngRange[ANG_RANGE_SIZE];
//  int m_errno;
//  double m_Mandelstam[MANDEL_SIZE];
//  double m_LorentzFac;

public:
  KCollision();  
  KCollision(KParticle& p1, KParticle& p2,
	     KParticle& p3, KParticle& p4);
  KCollision(KParticle* p1, KParticle* p2,
	     KParticle* p3, KParticle* p4);  
  KCollision(KFrame& init, KFrame& fin);
  KCollision(KFrame* init, KFrame* fin);  
  ~KCollision() {}

public:
  void SetParticle(KParticle& p1, KParticle& p2,
		   KParticle& p3, KParticle& p4);
  void SetParticle(KParticle* p1, KParticle* p2,
		   KParticle* p3, KParticle* p4);  
  void SetFrame(KFrame& init, KFrame& fin);
  void SetFrame(KFrame* init, KFrame* fin);  
  void SetInitParticle(KParticle& p1, KParticle& p2);
  void SetInitParticle(KParticle* p1, KParticle* p2);  
  void SetInitFrame(KFrame& init);
  void SetInitFrame(KFrame* init);    
  void SetFinParticle(KParticle& p3, KParticle& p4);
  void SetFinParticle(KParticle* p3, KParticle* p4);  
  void SetFinFrame(KFrame& fin);
  void SetFinFrame(KFrame* fin);  
//  void SetScattAng(double angle);
//  void SetScattAng(double minang, double maxang, double delta);
//  inline void SetRecAng(double angle){ClearAng();m_RecAng=angle;}
//  void SetRecAng(double minang, double maxang, double delta);
//  inline double GetScattAng() {return m_ScattAng;}
//  void Scatt();
//  inline double MandelstamS() {return m_Mandelstam[S];}
//  inline double MandelstamT() {return m_Mandelstam[T];}
//  inline double MandelstamU() {return m_Mandelstam[U];}
//  K4Momentum GetMomentumLab(int NofP);
//  K4Momentum GetMomentumCM(int NofP);  
  
private:
  void Initialize();
//  void ClearAng();
//  void SetLorentzFac();
//  void SetMandelstam(); // S, T, U
//  void SetMandelstamS();
//  void SetMandelstamT();
//  void SetMandelstamU();  
};

#endif
