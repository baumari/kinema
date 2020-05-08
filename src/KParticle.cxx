#include <string>
#include <KParticle.hh>
#include <cmath>
#include <algorithm>
#include <KUtil.hh>
#include <cfloat>
#include <cstdio>
#include <KError.hh>
#include <iostream>

double KParticle::GetMass(std::string m_name)
{
  double mass=0;
  int f_find=0;
  for(unsigned int i=0;i<sizeof(ParticleData)/sizeof(_ParticleData);i++){
    if(ParticleData[i].name==KUtil::str_tolower(m_name)){
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

KParticle::KParticle(std::string name)
{
  m_name = name;
  m_mass = GetMass(name);
  m_ex = 0;
  m_p.Set(m_mass, 0, 0, 0);
  m_errno = 0;
}

KParticle::KParticle(double energy, K3Vector p)
{
  m_name = "unknown";
  m_mass = sqrt(pow(energy, 2)-pow(p.Norm(), 2));
  m_ex = 0;  
  m_p.Set(energy, p);
  m_errno = 0;
}

KParticle::KParticle(double mass)
{
  m_name = "unknown";
  m_mass = mass;
  m_ex = 0;  
  m_p.Set(m_mass, 0, 0, 0);
  m_errno = 0;  
}

KParticle::KParticle(std::string name, double kin_energy,
	   double dir_x, double dir_y, double dir_z)
{
  m_name = name;
  m_mass = GetMass(name);
  m_ex = 0;  
  double energy = kin_energy+m_mass;
  KUtil::Normalize(sqrt(pow(energy,2)-pow(m_mass,2)),
		   dir_x, dir_y, dir_z);
  m_p.Set(energy, dir_x, dir_y, dir_z);
  m_errno = 0;  
}

KParticle::KParticle(double mass, double kin_energy,
	   double dir_x, double dir_y, double dir_z)
{
  m_name = "unknown";
  m_mass = mass;
  m_ex = 0;  
  double energy = kin_energy+m_mass;
  KUtil::Normalize(sqrt(pow(energy,2)-pow(m_mass,2)),
		   dir_x, dir_y, dir_z);
  m_p.Set(energy, dir_x, dir_y, dir_z);
  m_errno = 0;  
}

KParticle::KParticle(std::string name, double kin_energy, K3Vector p)
{
  m_name = name;
  m_mass = GetMass(name);
  m_ex = 0;  
  double energy = kin_energy+m_mass;
  KUtil::Normalize(sqrt(pow(energy, 2)-pow(m_mass, 2)), p);
  m_p.Set(energy, p);
  m_errno = 0;  
}

KParticle::KParticle(double mass, double kin_energy, K3Vector p)
{
  m_name = "unknown";
  m_mass = mass;
  m_ex = 0;  
  double energy = kin_energy+m_mass;
  KUtil::Normalize(sqrt(pow(energy, 2)-pow(m_mass, 2)), p);
  m_p.Set(energy, p);
  m_errno = 0;  
}

KParticle::KParticle(std::string name, double kin_energy)
{
  m_name = name;
  m_mass = GetMass(name);
  m_ex = 0;  
  double energy = kin_energy + m_mass;
  m_p.Set(energy, sqrt(pow(energy, 2)-pow(m_mass, 2)), 0, 0);
  m_errno = 0;  
}

KParticle::KParticle(double mass, double kin_energy)
{
  m_name = "unknown";
  m_mass = mass;
  m_ex = 0;  
  double energy = kin_energy + m_mass;
  m_p.Set(energy, sqrt(pow(energy, 2)-pow(m_mass, 2)), 0, 0);
  m_errno = 0;    
}

bool KParticle::IsErr()
{
  if(m_errno==0) return false;
  else{
    fprintf(stderr, "Error in KParticle. Error_Code: %d\n", m_errno);
    return true;
  }
}

void KParticle::SetParticle(std::string name)
{
  m_name = name;
  m_mass = GetMass(m_name);
}

void KParticle::SetDirection(double dirx, double diry, double dirz)
{
  KUtil::Normalize(sqrt(pow(E(),2)-pow(Mass(),2)), dirx, diry, dirz);
  m_p.Set(E(), dirx, diry, dirz);
}

void KParticle::SetDirection(const K3Vector& dir)
{
  double dirx, diry, dirz;
  dirx = dir.X(); diry = dir.Y(); dirz = dir.Z();
  KUtil::Normalize(sqrt(pow(E(),2)-pow(Mass(),2)), dirx, diry, dirz);
  m_p.Set(E(), dirx, diry, dirz);
}

void KParticle::SetMomentum(double px, double py, double pz) // energy will be recalced
{
  m_p.Set(sqrt(pow(m_mass,2)+pow(px,2)+pow(py,2)+pow(pz,2)),
	  px, py, pz);
}

void KParticle::SetMomentumComponent(double energy,
				     double px, double py, double pz)
{
  if(energy - Mass() < -KUtil::LOOSE_EPSILON){
    fprintf(stderr,
	    "KParticle::SetMomentumComponent Energy must be larger than Mass!!\n");
    m_errno = KError::INVALID_ARGUMENT;
    return ;    
  }
  m_p.Set(energy, px, py, pz);
  m_mass = m_p.Invaliant();
}

void KParticle::SetMomentum(const K3Vector& p) // energy will be recalced
{
  m_p.Set(sqrt(pow(m_mass,2)+pow(p.Norm(),2)), p);
}

void KParticle::SetMomentumComponent(double energy, const K3Vector& p)
{
  if(energy - Mass() < -KUtil::LOOSE_EPSILON){
    fprintf(stderr,
	    "KParticle::SetMomentumComponent Energy must be larger than Mass!!\n");
    m_errno = KError::INVALID_ARGUMENT;
    return ;    
  }  
  m_p.Set(energy, p);
  m_mass = m_p.Invaliant();
}

void KParticle::SetT(double kin_energy)
{
  if(kin_energy < -KUtil::LOOSE_EPSILON){
    fprintf(stderr,
	    "Eenrgy must be larger than 0 in KParticle::SetKinEnergy!!\n");
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

void KParticle::SetEnergy(double energy)
{
  if(energy - Mass() < -KUtil::LOOSE_EPSILON){
    fprintf(stderr,
	    "KParticle::SetEnergy Energy must be larger than Mass!!\n");
    m_errno = KError::INVALID_ARGUMENT;
    return ;
  }
  double px, py, pz;
  px = m_p.X(); py = m_p.Y(); pz = m_p.Z();
  KUtil::Normalize(sqrt(pow(energy, 2)-pow(m_mass, 2)),
		   px, py, pz);
  m_p.Set(energy, px, py, pz);
}

void KParticle::
SetTDirection(double kin_energy, double dirx, double diry, double dirz)
{
  if(kin_energy < -KUtil::LOOSE_EPSILON){
    fprintf(stderr,
	    "Eenrgy must be larger than 0 in KParticle::SetKinEnergy!!\n");
    m_errno = KError::INVALID_ARGUMENT;
    return ;
  }  
  double energy = m_mass + kin_energy;
  KUtil::Normalize(sqrt(pow(energy, 2)-pow(m_mass, 2)),
		   dirx, diry, dirz);
  m_p.Set(energy, dirx, diry, dirz);
}

void KParticle::
SetTDirection(double kin_energy, K3Vector dir)
{
  if(kin_energy < -KUtil::LOOSE_EPSILON){
    fprintf(stderr,
	    "Eenrgy must be larger than 0 in KParticle::SetKinEnergy!!\n");
    m_errno = KError::INVALID_ARGUMENT;
    return ;
  }  
  double energy = m_mass + kin_energy;
  KUtil::Normalize(sqrt(pow(energy, 2)-pow(m_mass, 2)), dir);
  m_p.Set(energy, dir);
}

void KParticle::
SetEnergyDirection(double energy, double dirx, double diry, double dirz)
{
  if(energy - Mass() < -KUtil::LOOSE_EPSILON){
    fprintf(stderr,
	    "KParticle::SetEnergyDirection Energy must be larger than Mass!!\n");
    m_errno = KError::INVALID_ARGUMENT;
    return ;
  }  
  KUtil::Normalize(sqrt(pow(energy, 2)-pow(m_mass, 2)),
		   dirx, diry, dirz);
  m_p.Set(energy, dirx, diry, dirz);
}

void KParticle::
SetEnergyDirection(double energy, K3Vector dir)
{
  if(energy - Mass() < -KUtil::LOOSE_EPSILON){
    fprintf(stderr,
	    "KParticle::SetEnergyDirection Energy must be larger than Mass!!\n");
    m_errno = KError::INVALID_ARGUMENT;
    return ;
  }  
  KUtil::Normalize(sqrt(pow(energy, 2)-pow(m_mass, 2)), dir);
  m_p.Set(energy, dir);
}

KParticle& KParticle::operator=(const KParticle& rhs)
{
  m_name = rhs.Name();
  m_mass = rhs.Mass();
  m_errno = rhs.ErrorNum();
  m_p = rhs.P();
  return *this;
}

void KParticle::BoostX(double beta)
{
  K4Momentum P = m_p;
  P.BoostX(beta);
  SetMomentumComponent(P.E(), P.P());
}

void KParticle::BoostY(double beta)
{
  K4Momentum P = m_p;
  P.BoostY(beta);
  SetMomentumComponent(P.E(), P.P());  
}

void KParticle::BoostZ(double beta)
{
  K4Momentum P = m_p;
  P.BoostZ(beta);
  SetMomentumComponent(P.E(), P.P());    
}

void KParticle::Boost(K3Vector beta)
{
  K4Momentum P = m_p;
  P.Boost(beta);
  SetMomentumComponent(P.E(), P.P());      
}

void KParticle::Boost(double x, double y, double z)
{
  K4Momentum P = m_p;
  P.Boost(x, y, z);
  SetMomentumComponent(P.E(), P.P());        
}

void KParticle::Show()
{
  printf("%lf: (%lf %lf %lf %lf)\n",
	 m_mass, E(), X(), Y(), Z());
  printf("Ex: %lf\n", Ex());
}


void KParticle::Init()
{
  m_name = "";
  m_mass = 0;
  m_ex = 0;  
  m_errno = 0;
  m_p.Set(0, 0, 0, 0);
}

void KParticle::SetEx(double ex)
{
  m_mass -= m_ex;
  m_ex = ex;
  m_mass += m_ex;
}
