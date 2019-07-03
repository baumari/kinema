#include <string>
#include <KParticle.hh>
#include <cmath>
#include <algorithm>
#include <KUtil.hh>
#include <cfloat>
#include <cstdio>

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

KParticle::Momentum::Momentum(double energy,
			     double px, double py, double pz)
{
  m_p[ENERGY]=energy;
  KUtil::Normalize(1,px,py,pz);
  m_p[PX]=px;
  m_p[PY]=py;
  m_p[PZ]=pz;  
}

KParticle::Momentum::Momentum(double energy)
{
  m_p[ENERGY]=energy;
  m_p[PX]=0;
  m_p[PY]=0;
  m_p[PZ]=1;
}

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
    m_errno=PARTICLE_DATA_NOT_FOUND;
  }
  return mass;
}

double KParticle::GetBeta()
{
  return sqrt(1-(m_mass/m_Momentum.GetE())*
	      (m_mass/m_Momentum.GetE()));
}

double KParticle::GetGamma()
{
  if(m_mass < DBL_EPSILON){
    fprintf(stderr, "Division by Zero in KParticle::GetGamma!!\n");
    m_errno = ZERO_DIVISION;
    return EXIT_FAILURE;
  }
  return m_Momentum.GetE()/m_mass;
}

bool KParticle::IsFail()
{
  if(m_errno==0) return false;
  else{
    fprintf(stderr, "Error in KParticle. Error_Code: %d\n", m_errno);
    return true;
  }
}

void KParticle::SetMomentum(double px, double py, double pz)
{
  m_Momentum.SetPx(px);
  m_Momentum.SetPy(py);
  m_Momentum.SetPz(pz);
  m_Momentum.SetE(sqrt(px*px+py*py+pz*pz+m_mass*m_mass));
}

void KParticle::SetEnergy(double kin_energy)
{
  if(kin_energy < 0){
    fprintf(stderr,
	    "Eenrgy must be larger than 0 in Momentum::SetEnergy!!\n");
    m_errno = INVALID_ARGUMENT;    
  }
  m_Momentum.SetE(kin_energy+m_mass);
  double norm=sqrt((kin_energy+m_mass)*(kin_energy+m_mass)-m_mass*m_mass);
  double px, py, pz;
  px=m_Momentum.GetPx(); py=m_Momentum.GetPy(); pz=m_Momentum.GetPz();
  KUtil::Normalize(norm, px, py, pz);
  m_Momentum.SetPx(px); m_Momentum.SetPy(py); m_Momentum.SetPz(pz);
}

void KParticle::
SetEnergyDirection(double kin_energy, double px, double py, double pz)
{
  m_Momentum.SetE(kin_energy+m_mass);
  double norm=sqrt((kin_energy+m_mass)*(kin_energy+m_mass)-m_mass*m_mass);
  KUtil::Normalize(norm, px, py, pz);
  m_Momentum.SetPx(px); m_Momentum.SetPy(py); m_Momentum.SetPz(pz);
}
