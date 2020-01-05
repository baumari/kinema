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

KParticle::KParticle(double mass, double kin_energy,
	   double dir_x, double dir_y, double dir_z)
{
  m_name = "unknown";
  m_mass = mass;
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

KParticle::KParticle(double mass, double kin_energy, K3Vector p)
{
  m_name = "unknown";
  m_mass = mass;
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

KParticle::KParticle(double mass, double kin_energy)
{
  m_name = "unknown";
  m_mass = mass;
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

void KParticle::SetMomentumNoScale(double px, double py, double pz)
{
  m_p.Set(E(), px, py, pz);
}

void KParticle::SetMomentumNoScale(const K3Vector& p)
{
  m_p.Set(E(), p);
}

void KParticle::SetMomentum(double px, double py, double pz)
{
  m_p.Set(sqrt(pow(m_mass,2)+pow(px,2)+pow(py,2)+pow(pz,2)),
	  px, py, pz);
}

void KParticle::SetMomentumComponent(double energy,
				     double px, double py, double pz)
{
  if(energy - Mass() < 0){
    fprintf(stderr,
	    "KParticle::SetEnergyDirection Energy must be larger than Mass!!\n");
    m_errno = KError::INVALID_ARGUMENT;
    return ;    
  }
  m_p.Set(energy, px, py, pz);
}

void KParticle::SetMomentum(const K3Vector& p)
{
  m_p.Set(sqrt(pow(m_mass,2)+pow(p.Norm(),2)), p);
}

void KParticle::SetMomentumComponent(double energy, const K3Vector& p)
{
  if(energy - Mass() < 0){
    fprintf(stderr,
	    "KParticle::SetEnergyDirection Energy must be larger than Mass!!\n");
    m_errno = KError::INVALID_ARGUMENT;
    return ;    
  }  
  m_p.Set(energy, p);
}

void KParticle::SetT(double kin_energy)
{
  if(kin_energy < 0){
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
  if(energy - Mass() < 0){
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
  if(kin_energy < 0){
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
SetTDirection(double kin_energy, K3Vector& dir)
{
  if(kin_energy < 0){
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
  if(energy - Mass() < 0){
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
SetEnergyDirection(double energy, K3Vector& dir)
{
  if(energy - Mass() < 0){
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

//KParticle& KParticle::BoostX(double beta)
//{
//  double gamma = KUtil::BetaToGamma(beta);
//  SetKinEnergyDirection(m_p.E()*gamma-beta*gamma*m_p.X(),
//		     -beta*gamma*m_p.E()+gamma*m_p.X(), m_p.Y(), m_p.Z());
//  return *this;
//}
//
//KParticle& KParticle::BoostY(double beta)
//{
//  double gamma = KUtil::BetaToGamma(beta);
//  SetKinEnergyDirection(m_p.E()*gamma-beta*gamma*m_p.Y(), m_p.X(),
//		     -beta*gamma*m_p.E()+gamma*m_p.Y(), m_p.Z());
//  return *this;  
//}
//
//KParticle& KParticle::BoostZ(double beta)
//{
//  double gamma = KUtil::BetaToGamma(beta);
//  SetKinEnergyDirection(m_p.E()*gamma-beta*gamma*m_p.Z(), m_p.X(), m_p.Y(),
//		     -beta*gamma*m_p.E()+gamma*m_p.Z());
//  return *this;  
//}
//
//KParticle& KParticle::Boost(const K3Vector& beta)
//{
//  double gamma = KUtil::BetaToGamma(beta);
//  
//  return *this;
//}
//
//KParticle& KParticle::Boost(double x, double y, double z)
//{
//
//  return *this;
//}

void KParticle::Show()
{
  printf("%lf: (%lf %lf %lf %lf)\n",
	 m_mass, E(), X(), Y(), Z());
}

K3Vector KParticle::GetBeta()
{
  return P().P()/E();
}

double KParticle::GetGamma()
{
  return E()/Mass();
}
