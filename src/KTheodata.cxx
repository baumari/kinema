#include <KTheodata.hh>
#include <TSpline.h>
#include <stdio.h>
#include <algorithm>

KTheodata::KTheodata() :
  fspline(nullptr) {}  

KTheodata::~KTheodata(){
  //  if(fspline) delete fspline;
}

void KTheodata::Print(){
  printf("theodata\n");  
  for(int i=0;i!=GetN();++i){
    printf("%lf %lf \n",fx[i],fy[i]);    
  }
  printf("\n\n");  
}

void KTheodata::PrintCorrect(){
  printf("theodata_corrected\n");  
  for(int i=0;i!=GetNCorrect();++i){
    printf("%lf %lf \n",fx_correct[i],fy_correct[i]);    
  }
  printf("\n\n");  
}

double KTheodata::MakeSpline(double *x, double *par){
  double xx = x[0];
  TSpline3 sp3("Spline Theoretical Data", &fx[0], &fy[0], GetN());
  return sp3.Eval(xx)*par[0];
}

KTheodata::SPLINE KTheodata::GetSpline(){
  fspline = &KTheodata::MakeSpline;
  return fspline;
}

double KTheodata::GetfxMin(){
  return *std::min_element(fx.begin(), fx.end());
}

double KTheodata::GetfxMax(){
  return *std::max_element(fx.begin(), fx.end());
}

