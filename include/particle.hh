#ifndef _PARTICLE_HH
#define _PARTICLE_HH

#include <FVector.hh>

class particle
{
private:
  std::string m_name;
  FVector m_p;
  double getmass(std::string m_name);  
public:
  particle();
  ~particle();
  particle(char* name, double KE);
  double getbeta();
  double getgamma();
};

#endif
