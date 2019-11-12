#ifndef _KPARTICLE_HH
#define _KPARTICLE_HH

#include <K4Momentum.hh>
#include <string>
#include <KUtil.hh>
#include <cmath>

/* Unit of energy is MeV */
/* Unit of momentum is MeV/c */

/************* table of particle **************/
/*** add information below if you necessary ***/

const double AMU = 931.478;

typedef struct {
  const std::string name;
  const double massex;
  const int z;  
  const int a;
} _ParticleData;

const _ParticleData ParticleData[]={
  {"h",7.28897,1,1},        
  {"p",7.28897,1,1},
  {"proton",7.28897,1,1},
  {"hydrogen",7.28897,1,1},
  {"4he",2.42492,2,4},
  {"a",2.42492,2,4},
  {"alpha",2.42492,2,4},
  {"helium",2.42492,2,4},
  {"6li",14.08688,3,6},
  {"8be",4.94167,4,8},
  {"t",14.94981,1,3},
  {"d",13.13572,1,2},
  {"12c",0,6,12},
  {"24mg",-13.93357,12,24},
  {"20ne",-7.04193,10,20},
  {"3he",14.93121,2,3},
  {"n",8.0713,0,1}
};
/************* table of particle **************/

class KParticle
{
private:
  std::string m_name;
  double m_mass;
  int m_errno;
  K4Momentum m_p;

private:
  double GetMass(std::string m_name);

public:
  KParticle() {}
  // The norm of momentum is re-defined by energy 
  KParticle(std::string name, double kin_energy,
	    double dir_x, double dir_y, double dir_z);
  KParticle(std::string name, double kin_energy, K3Vector p);
  KParticle(std::string name, double kin_energy);
  ~KParticle() {}

public:
  bool IsErr();  

  double Beta(); //please check from which frame you see
  double Gamma(); //please check from which frame you see

  inline double E() const {return m_p.E();}
  inline double Px() const {return m_p.Px();}
  inline double Py() const {return m_p.Py();}
  inline double Pz() const {return m_p.Pz();}
  inline K4Momentum P() const {return m_p;}
  inline double Mass() const {return m_mass;}
  inline std::string Name() const {return m_name;}
  /* Function SetDirection can define only direction of particle. */
  /* The amaount of momentum is defined by energy */
  void SetDirection(double px, double py, double pz);
  void SetDirection(const K3Vector& p);
  /* Function SetMomentum can define both direction and amount. */
  /* The energy will be recalced. */  
  void SetMomentum(double px, double py, double pz);
  void SetMomentum(const K3Vector& p);
  void SetEnergy(double kin_energy);// direction same as previous is set. 
  void SetEnergyDirection(double kin_energy,
			  double px, double py, double pz);
  inline int ErrorNum() const {return m_errno;}  
  KParticle& operator=(const KParticle& rhs);
};

#endif
