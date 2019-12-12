#include <KFrame.hh>

KFrame::KFrame(KParticle &p1, KParticle &p2)
{
  m_p1 = p1; m_p2 = p2;
}

void KFrame::Show()
{
  m_p1.P().Show();
}
