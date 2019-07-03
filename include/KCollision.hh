#ifndef _KCOLLISION_HH
#define _KCOLLISION_HH

#include <KParticle.hh>

class KCollision{
private:
  KParticle* m_p1;
  KParticle* m_p2;

public:
  KCollision() :
    m_p1(NULL),
    m_p2(NULL) {}
  KCollision(KParticle& p1, KParticle& p2) :
    m_p1(&p1),
    m_p2(&p2) {}
  ~KCollision() {}

public:
  inline void SetInitParticle(KParticle& p1, KParticle& p2){
    m_p1=&p1;m_p2=&p2;
  }
  

public: //for debug
  inline KParticle* GetAddr1() {return m_p1;}
  inline KParticle* GetAddr2() {return m_p2;}  

  
};

#endif
