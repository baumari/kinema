#ifndef _KFRAME_HH
#define _KFRAME_HH

#include <KParticle.hh>

class KFrame
{
private:
  KParticle m_p1;
  KParticle m_p2;

public:
  KFrame() {}
  KFrame(KParticle &p1, KParticle &p2);
  ~KFrame() {}

public:
  void Show();
};

#endif
