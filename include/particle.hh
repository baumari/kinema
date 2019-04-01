#ifndef _PARTICLE_HH
#define _PARTICLE_HH

#include <FVector.hh>

class particle
{
private:
  std::string m_name;
  double m_mass;
  double m_kine;
  FVector p;
public:
  particle();
  particle(char*, double);
};

#endif
