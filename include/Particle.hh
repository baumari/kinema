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

  public:
    inline double GetE() {return m_p[ENERGY];}
    inline double GetPx() {return m_p[PX];}
    inline double GetPy() {return m_p[PY];}
    inline double GetPz() {return m_p[PZ];}
    inline void SetE(double kin_energy) {m_p[ENERGY] = kin_energy;}
    inline void SetPx(double px) {m_p[PX] = px;}
    inline void SetPy(double py) {m_p[PY] = py;}
    inline void SetPz(double pz) {m_p[PZ] = pz;}    
  };

private:
  std::string m_name;
  double m_mass;
  int m_errno;
  Momentum m_Momentum;  
  enum Error_Code { 
    PARTICLE_DATA_NOT_FOUND=1,
    ZERO_DIVISION,
    INVALID_ARGUMENT,
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
  bool Fail();  

  double GetBeta();
  double GetGamma();

  inline double GetEnergy() {return m_Momentum.GetE();}
  inline double GetPx() {return m_Momentum.GetPx();}
  inline double GetPy() {return m_Momentum.GetPy();}
  inline double GetPz() {return m_Momentum.GetPz();}
  inline double GetMass() {return m_mass;}
  void SetMomentum(double px, double py, double pz);// non-normalization, energy will be recalc.
  void SetEnergy(double kin_energy);// direction same as previous is set. 
  void SetEnergyDirection(double kin_energy,
			  double px, double py, double pz);
};

#endif
