#include <KCollision.hh>
#include <cstdio>
#include <cstdlib>
#include <KError.hh>

void KCollision::SetScattAng(double minang,
			     double maxang, double delta)
{
  ClearAng();
  m_AngRange[MIN]=minang; m_AngRange[MAX]=maxang;
  m_AngRange[DELTA]=delta;
  m_ScattAng=m_AngRange[MIN];
}

void KCollision::SetRecAng(double minang,
			   double maxang, double delta)
{
  ClearAng();
  m_AngRange[MIN]=minang; m_AngRange[MAX]=maxang;
  m_AngRange[DELTA]=delta;
  m_RecAng=m_AngRange[MIN];  
}

void KCollision::ClearAng()
{
  m_ScattAng=0;m_RecAng=0;
  for(int i=0;i<ANG_RANGE_SIZE;i++) m_AngRange[i]=0;
}

void KCollision::Scatt()
{
  if(m_p[P1]==NULL || m_p[P2] == NULL){
    fprintf(stderr,
	    "Nullptr assigned to initial partilices in KCollision!!\n");
    m_errno=KError::NULLPTR_FOUND;
    return ;
  }else if(m_p[P3] == NULL || m_p[P4] == NULL){
    fprintf(stderr,
	    "Nullptr assigned to final partilices in KCollision!!\n");
    m_errno=KError::NULLPTR_FOUND;
    return;
  }

}
