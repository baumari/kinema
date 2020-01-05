#ifndef _KFRAME_HH
#define _KFRAME_HH

#include <KParticle.hh>
#include <K3Vector.hh>

class KFrame
{  
private:
  enum KFRAME{_P1, _P2, SIZE};

private:
  KParticle *m_p[SIZE];
  double m_Lorentzfac;
  K3Vector m_Beta; 

public:
  KFrame() {}
  KFrame(KParticle &p1, KParticle &p2);
  KFrame(KParticle *p1, KParticle *p2);  
  ~KFrame() {}

public:
  void SetParticle(KParticle &p1, KParticle &p2);
  void SetParticle(KParticle *p1, KParticle *p2);  
  void CM();
  void Show();
  KParticle* GetParticle(int No); 

private:
  
};

#endif
