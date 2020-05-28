#ifndef K_FRAME_HH
#define K_FRAME_HH

#include <KParticle.hh>
#include <K3Vector.hh>

class KFrame
{  
private:
  enum KFRAME{_P1, _P2, SIZE};

private:
  KParticle *m_p[SIZE];
  K3Vector m_Beta; // Beta vector used for transformation is stored

public:
  KFrame() {}
  KFrame(KParticle &p1, KParticle &p2);
  KFrame(KParticle *p1, KParticle *p2);  
  ~KFrame() {}

public:
  void SetParticle(KParticle &p1, KParticle &p2);
  void SetParticle(KParticle *p1, KParticle *p2);  
  void CM();
  inline K3Vector GetBeta(){return m_Beta;}
  void Boost(K3Vector &beta);
  void Show();

private:
  
};

#endif
