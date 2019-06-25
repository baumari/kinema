#include <string>
#include <particle.hh>
#include <myerrno.hh>
#include <cmath>
#include <algorithm>

/************* table of particle **************/
/*** add information below if you necessary ***/

const double AMU = 931.478;

typedef struct {
  const std::string name;
  const double massex;
  const int z;  
  const int a;
} pdata;

pdata pd[]={
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

Particle::Particle()
{
  m_name="";
}
Particle::~Particle(){}

Particle::Particle(std::string name, double kine)
{
  double mass;
  std::transform(name.begin(), name.end(), name.begin(), tolower);
  m_name=name;
  mass=GetMass(m_name);
  m_p.inite(mass, kine);
  m_p.update();
}

double Particle::GetMass(std::string m_name)
{
  double mass=-1;
  int f_find=0;
  for(unsigned int i=0;i<sizeof(pd)/sizeof(pdata);i++){
    if(pd[i].name==m_name){
      mass=pd[i].massex+(double)pd[i].a*AMU;
      f_find++;
      break;
    }
  }
  if(!f_find){
    fprintf(stderr, "particle data not found.. %s\n",m_name.c_str());
    fprintf(stderr, "you should add particle info (particle.cxx) at first!!!\n");
    merrno=MEPART_NOTFOUND;
  }
  return mass;
}

double Particle::GetBeta()
{
  return sqrt(1-(m_p.mass()/m_p.e())*(m_p.mass()/m_p.e()));
}

double Particle::GetGamma()
{
  return m_p.e()/m_p.mass();
}
