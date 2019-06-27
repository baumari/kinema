#include <cmath>
#include <iostream>
#include <cstdio>
#include <Util.hh>

void Norm(double norm,
	  double& x, double& y, double& z) // normalization 
{
  double fac=norm/sqrt(x*x+y*y+z*z);
  x*=fac; y*=fac; z*=fac;
}

void Norm(double norm,
	  double& x, double& y)
{
  double fac=norm/sqrt(x*x+y*y);
  x*=fac; y*=fac;  
}

void Usage(){
  printf("Usage: ./kinema [-g] [-r val] p1 p2 p3 p4 Ebeam Ex\n");
  printf("   -g: Graph will be displayed.\n");
  printf("   -r: Recoil kinetic energy.\n");
  printf("Unit of energy is [MeV].\n\n");
  printf("Output: E3 E4 theta3 theta4 \n");
  return;
}
