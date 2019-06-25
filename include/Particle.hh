#ifndef _PARTICLE_HH
#define _PARTICLE_HH

class Particle
{
private:
  class Momentum
  {
  private:
    enum Momentum_Component{
      ENERGY,
      PX,
      PY,
      PZ,

      SIZE,
    };

  private:
    double m_p[SIZE];

  public:
    Momentum() : m_p() {}
    Momentum(double kin_energy); // default direction (0,0,1)
    Momentum(double kin_energy, double px, double py, double pz);
    ~Momentum() {}

  };

private:
  std::string m_name;
  double m_mass;
  int m_errno;
  Momentum m_Momentum;  
  enum Error_Code { 
    PARTICLE_NOT_FOUND=1,
  };

private:
  double GetMass(std::string m_name);  

public:
  Particle() :
    m_name(""),
    m_mass(),
    m_errno(),
    m_Momentum() {}
  Particle(std::string name, double kin_energy,
	   double px, double py, double pz) :
    m_name(name),
    m_mass(GetMass(name)),
    m_errno(),
    m_Momentum(m_mass+kin_energy, px, py, pz) {}
  Particle(std::string name, double kin_energy) :
    m_name(name),
    m_mass(GetMass(name)),
    m_errno(),
    m_Momentum(m_mass+kin_energy) {}

  ~Particle() {}

public:
//  double GetE();
//  double GetPx();
//  double GetPy();
//  double GetPz();
//  double GetBeta();
//  double GetGamma();
  bool Fail();
//  double GetMass();
//  void SetMomentum(double px, double py, double pz);
//  void SetEnergy(double energy);  // direction is set same as previous
//  void SetEnergyDirection(double energy,
//			  double px, double py, double pz);
};

#endif
