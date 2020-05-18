#include <KExpdata.hh>
#include <stdio.h>
#include <sstream>
#include <cstdlib>
#include <iostream>

KExpdata::KExpdata() {}
KExpdata::KExpdata(std::string& filename){
  ifs.open(filename.c_str());
  if(ifs.fail()){
    std::cout << "Fail to open " << filename << std::endl;
    std::exit(EXIT_FAILURE);
  }
  std::string sLine;
  double x, y, x_err, y_err;
  while(std::getline(ifs, sLine)){
    std::stringstream ssLine(sLine);
    ssLine >> x >> y >> y_err >> x_err;
    fx.push_back(x); fy.push_back(y);
    fx_err.push_back(x_err); fy_err.push_back(y_err);
  }
}

KExpdata::KExpdata(char *filename){
  ifs.open(filename);
  if(ifs.fail()){
    std::cout << "Fail to open " << filename << std::endl;
    std::exit(EXIT_FAILURE);
  }
  std::string sLine;
  double x, y, x_err, y_err;
  while(std::getline(ifs, sLine)){
    std::stringstream ssLine(sLine);
    ssLine >> x >> y >> y_err >> x_err;
    fx.push_back(x); fy.push_back(y);
    fx_err.push_back(x_err); fy_err.push_back(y_err);
  }
}

KExpdata::~KExpdata() {ifs.close();}
void KExpdata::Print(){
  printf("expdata\n");
  for(int i=0;i!=GetN();++i){
    printf("%lf %lf %lf %lf\n",fx[i],fy[i],fx_err[i],fy_err[i]);
  }
  printf("\n\n");
}

KExpdataCS::KExpdataCS() {}
KExpdataCS::KExpdataCS(std::string& filename) {
  ifs.open(filename.c_str());
  if(ifs.fail()){
    std::cout << "Fail to open " << filename << std::endl;
    std::exit(EXIT_FAILURE);
  }
  std::string sLine;
  double x, y, x_err, y_err, x_width;
  while(std::getline(ifs, sLine)){
    std::stringstream ssLine(sLine);
    ssLine >> x >> y >> y_err >> x_width >> x_err;
    fx.push_back(x); fy.push_back(y);
    fx_err.push_back(x_err); fy_err.push_back(y_err);
    fx_width.push_back(x_width);
  }  
}

KExpdataCS::KExpdataCS(char *filename){
  ifs.open(filename);
  if(ifs.fail()){
    std::cout << "Fail to open " << filename << std::endl;
    std::exit(EXIT_FAILURE);
  }
  std::string sLine;
  double x, y, x_err, y_err, x_width;
  while(std::getline(ifs, sLine)){
    std::stringstream ssLine(sLine);
    ssLine >> x >> y >> y_err >> x_width >> x_err;
    fx.push_back(x); fy.push_back(y);
    fx_err.push_back(x_err); fy_err.push_back(y_err);
    fx_width.push_back(x_width);
  }  
}

KExpdataCS::~KExpdataCS() {ifs.close();}
void KExpdataCS::Print(){
  printf("expdataCS\n");  
  for(int i=0;i!=GetN();++i){
    printf("%lf %lf %lf %lf %lf\n",fx[i],fy[i],fx_err[i],fy_err[i],fx_width[i]);    
  }
  printf("\n\n");  
}

