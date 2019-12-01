#include <string>
#include <KParticle.hh>
#include <cmath>
#include <algorithm>
#include <KUtil.hh>
#include <cfloat>
#include <cstdio>
#include <KError.hh>

double KParticle::GetMass(std::string m_name)
{
  double mass=0;
  int f_find=0;
  for(unsigned int i=0;i<sizeof(ParticleData)/sizeof(_ParticleData);i++){
    if(ParticleData[i].name==m_name){
      mass=ParticleData[i].massex+(double)ParticleData[i].a*AMU;
      f_find++;
      break;
    }
  }
  if(!f_find){
    fprintf(stderr, "particle data not found.. %s\n",m_name.c_str());
    fprintf(stderr, "you should add particle info (KParticle.cxx) at first!!!\n");
    m_errno=KError::PARTICLE_DATA_NOT_FOUND;
  }
  return mass;
}

KParticle::KParticle(std::string name, double kin_energy,
	   double dir_x, double dir_y, double dir_z)
{
  m_name = name;
  m_mass = GetMass(name);
  double energy = kin_energy+m_mass;
  KUtil::Normalize(sqrt(pow(energy,2)-pow(m_mass,2)),
		   dir_x, dir_y, dir_z);
  m_p.Set(energy, dir_x, dir_y, dir_z);
}

KParticle::KParticle(std::string name, double kin_energy, K3Vector p)
{
  m_name = name;
  m_mass = GetMass(name);
  double energy = kin_energy+m_mass;
  KUtil::Normalize(sqrt(pow(energy, 2)-pow(m_mass, 2)), p);
  m_p.Set(energy, p);
}

KParticle::KParticle(std::string name, double kin_energy)
{
  m_name = name;
  m_mass = GetMass(name);
  double energy = kin_energy + m_mass;
  m_p.Set(energy, sqrt(pow(energy, 2)-pow(m_mass, 2)), 0, 0);
}

bool KParticle::IsErr()
{
  if(m_errno==0) return false;
  else{
    fprintf(stderr, "Error in KParticle. Error_Code: %d\n", m_errno);
    return true;
  }
}

double KParticle::Beta()
{
  return sqrt(1-pow(1./Gamma(),2));
}

double KParticle::Gamma()
{
  if(m_mass < DBL_EPSILON){
    fprintf(stderr, "Division by Zero in KParticle::Gamma!!\n");
    m_errno = KError::ZERO_DIVISION;
    return EXIT_FAILURE;
  }
  return m_p.E()/m_mass;
}

void KParticle::SetDirection(double px, double py, double pz)
{
  KUtil::Normalize(m_p.P().Norm(), px, py, pz);
  m_p.P().Set(px, py, pz);
}

void KParticle::SetDirection(const K3Vector& p)
{
  double px, py, pz;
  px = p.X(); py = p.Y(); pz = p.Z();
  KUtil::Normalize(m_p.P().Norm(), px, py, pz);
  m_p.P().Set(px, py, pz); 
}

void KParticle::SetMomentum(double px, double py, double pz)
{
  m_p.Set(sqrt(pow(m_mass,2)+pow(px,2)+pow(py,2)+pow(pz,2)),
	  px, py, pz);
}

void KParticle::SetMomentum(const K3Vector& p)
{
  m_p.Set(sqrt(pow(m_mass,2)+pow(p.Norm(),2)), p);
}

void KParticle::SetEnergy(double kin_energy)
{
  if(kin_energy < 0){
    fprintf(stderr,
	    "Eenrgy must be larger than 0 in KParticle::SetEnergy!!\n");
    m_errno = KError::INVALID_ARGUMENT;
    return ;
  }
  double energy = m_mass + kin_energy;
  double px, py, pz;
  px = m_p.X(); py = m_p.Y(); pz = m_p.Z();
  KUtil::Normalize(sqrt(pow(energy, 2)-pow(m_mass, 2)),
		   px, py, pz);
  m_p.Set(energy, px, py, pz);
}

void KParticle::
SetEnergyDirection(double kin_energy, double px, double py, double pz)
{
  double energy = m_mass + kin_energy;
  KUtil::Normalize(sqrt(pow(energy, 2)-pow(m_mass, 2)),
		   px, py, pz);
  m_p.Set(energy, px, py, pz);
}

KParticle& KParticle::operator=(const KParticle& rhs)
{
  m_name = rhs.Name();
  m_mass = rhs.Mass();
  m_errno = rhs.ErrorNum();
  m_p = rhs.P();
  return *this;
}

void KParticle::BoostX(double gamma)
{
  double beta = KUtil::GammaToBeta(gamma);
  SetEnergyDirection(m_p.E()*gamma-beta*gamma*m_p.X(),
		     -beta*gamma*m_p.E()+gamma*m_p.X(), m_p.Y(), m_p.Z());
}

void KParticle::BoostY(double gamma)
{
  double beta = KUtil::GammaToBeta(gamma);
  SetEnergyDirection(m_p.E()*gamma-beta*gamma*m_p.Y(), m_p.X(),
		     -beta*gamma*m_p.E()+gamma*m_p.Y(), m_p.Z());
}

void KParticle::BoostZ(double gamma)
{
  double beta = KUtil::GammaToBeta(gamma);
  SetEnergyDirection(m_p.E()*gamma-beta*gamma*m_p.Z(), m_p.X(), m_p.Y(),
		     -beta*gamma*m_p.E()+gamma*m_p.Z());  
}
