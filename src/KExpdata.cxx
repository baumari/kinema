#include <KExpdata.hh>
#include <stdio.h>

KExpdata::KExpdata() {}
KExpdata::~KExpdata() {}
void KExpdata::Print(){
  printf("expdata\n");
  for(int i=0;i!=GetN();++i){
    printf("%lf %lf %lf %lf\n",fx[i],fy[i],fx_err[i],fy_err[i]);
  }
  printf("\n\n");
}

KExpdataCS::KExpdataCS() {}
KExpdataCS::~KExpdataCS() {}
void KExpdataCS::Print(){
  printf("expdataCS\n");  
  for(int i=0;i!=GetN();++i){
    printf("%lf %lf %lf %lf %lf\n",fx[i],fy[i],fx_err[i],fy_err[i],fx_width[i]);    
  }
  printf("\n\n");  
}

