#ifndef _PARTICLE_HH
#define _PARTICLE_HH

#include <FVector.hh>

class Particle
{
private:
  std::string m_name;
  FVector m_p;
  double GetMass(std::string m_name);  
public:
  Particle();
  ~Particle();
  Particle(std::string name, double KE);
  double GetBeta();
  double GetGamma();
  void SetParticle();
};

#endif
