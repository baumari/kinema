#include <KTheodata.hh>
#include <TSpline.h>
#include <stdio.h>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <iostream>

KTheodata::KTheodata() :
  fspline(nullptr) {}  

KTheodata::KTheodata(std::string& filename) :
  fspline(nullptr){Open(filename);}

KTheodata::KTheodata(char *filename) :
  fspline(nullptr){Open(filename);}

KTheodata::~KTheodata(){
  //  if(fspline) delete fspline;
  ifs.close();
}

void KTheodata::Open(std::string &filename){
  ifs.open(filename.c_str());
  if(ifs.fail()){
    std::cout << "Fail to open " << filename << std::endl;
    std::exit(EXIT_FAILURE);
  }
  std::string sLine;
  double x, y;
  while(std::getline(ifs, sLine)){
    std::stringstream ssLine(sLine);
    ssLine >> x >> y;
    fx.push_back(x); fy.push_back(y);
  }  
}

void KTheodata::Open(char *filename){
  ifs.open(filename);
  if(ifs.fail()){
    std::cout << "Fail to open " << filename << std::endl;
    std::exit(EXIT_FAILURE);
  }
  std::string sLine;
  double x, y;
  while(std::getline(ifs, sLine)){
    std::stringstream ssLine(sLine);
    ssLine >> x >> y;
    fx.push_back(x); fy.push_back(y);
  }    
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

void KTheodata::Scale(double factor){
  for(auto &x : fy) x*=factor;
  for(auto &x : fy_correct) x*=factor;
}
