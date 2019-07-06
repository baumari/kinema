#ifndef _KCOLLISION_HH
#define _KCOLLISION_HH

#include <KParticle.hh>

class KCollision{
private:
  enum Particle_Num {
    P1,
    P2,
    P3,
    P4,
	   
    PARTICLE_NUM_SIZE,
  };

  enum Ang_Range {
    MIN,
    MAX,
    DELTA,
		  
    ANG_RANGE_SIZE,
  };

private:
  KParticle* m_p[PARTICLE_NUM_SIZE];
  double m_ScattAng;
  double m_RecAng;
  double m_AngRange[ANG_RANGE_SIZE];
  int m_errno;

public:
  KCollision() :
    m_p(),
    m_ScattAng(),
    m_RecAng(),
    m_AngRange() {}    
  KCollision(KParticle& p1, KParticle& p2,
	     KParticle& p3, KParticle& p4) :
    m_ScattAng(),
    m_RecAng(),
    m_AngRange() {
    SetInitParticle(p1, p2);
    SetFinParticle(p3, p4);  
  }
  ~KCollision() {}

public:
  inline void SetInitParticle(KParticle& p1, KParticle& p2){
    m_p[P1]=&p1;m_p[P2]=&p2;
  }
  inline void SetFinParticle(KParticle& p3, KParticle& p4){
    m_p[P3]=&p3;m_p[P4]=&p4;
  }
  inline void SetScattAng(double angle){ClearAng();m_ScattAng=angle;}
  void SetScattAng(double minang, double maxang, double delta);
  inline void SetRecAng(double angle){ClearAng();m_RecAng=angle;}
  void SetRecAng(double minang, double maxang, double delta);
  void Scatt();
  inline double GetScattAng() {return m_ScattAng;}
  
public: //for debug
  inline KParticle* GetAddr1() {return m_p[P1];}
  inline KParticle* GetAddr2() {return m_p[P2];}

private:
  void ClearAng();

  
};

#endif
