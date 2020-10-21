#include <KTheodata.hh>
#include <TSpline.h>
#include <stdio.h>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <KUtil.hh>
#include <fstream>

void KTheodata::Open(std::string &filename){
  std::ifstream ifs(filename.c_str());
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
  std::ifstream ifs(filename);
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

void KTheodata::Print() const{
  printf("theodata\n");  
  for(int i=0;i!=GetN();++i){
    printf("%lf %lf \n",fx[i],fy[i]);    
  }
  printf("\n\n");  
}

void KTheodata::PrintCorrect() const{
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

double KTheodata::MakeSplineCorrect(double *x, double *par){
  double xx = x[0];
  TSpline3 sp3("Spline Theoretical Data",
	       &fx_correct[0], &fy_correct[0], GetNCorrect());
  return sp3.Eval(xx)*par[0];
}

KTheodata::SPLINE KTheodata::GetSpline(){
  fspline = &KTheodata::MakeSpline;
  return fspline;
}

double KTheodata::GetfxMin() const{
  return *std::min_element(fx.begin(), fx.end());
}

double KTheodata::GetfxMax() const{
  return *std::max_element(fx.begin(), fx.end());
}

double KTheodata::GetfxCorrectMin() const{
  return *std::min_element(fx_correct.begin(), fx_correct.end());
}

double KTheodata::GetfxCorrectMax() const{
  return *std::max_element(fx_correct.begin(), fx_correct.end());
}

void KTheodata::Scale(double factor){
  for(auto &x : fy) x*=factor;
  for(auto &x : fy_correct) x*=factor;
}

void KTheodata::Correction(const KExpdataCS &exp){
  const int nAveragePoint = 3;
  int nExpData = exp.GetN();
  std::vector<double> xtmp(nAveragePoint);
  std::vector<double> csdw(nAveragePoint);
  fx_correct = exp.fx;
  fy_correct.resize(nExpData);
  for(int i = 0; i != nExpData; ++i){
    double sum = 0;
    for(int j = 0; j != nAveragePoint; ++j){
      xtmp[j] = (exp.fx[i]-exp.fx_width[i])
	+(double)j*2.*exp.fx_width[i]/(nAveragePoint-1.0);
      csdw[j] = Interpolate(xtmp[j]); // linear interpolation
      sum += csdw[j];
    }
    fy_correct[i] = sum/(double)nAveragePoint;
  }
}

double KTheodata::Interpolate(double x){ // linear interpolation
  double val = 0;
  if(x < GetfxMin() || x > GetfxMax()){
    std::cerr << "Requested angle " << x << " is out of range." << std::endl;
    std::exit(EXIT_FAILURE);
  }
  for(int i = 1; i != GetN(); ++i){
    if(x <= fx[i]){
      val = (fy[i]-fy[i-1])/(fx[i]-fx[i-1])
	*(x-fx[i-1])+fy[i-1];
      break;
    }
  }
  return val;
}

double KTheodata::Getf(double *x, double *par) const{
  return par[0]*fy[FindIndex(fx, x[0])];
}

double KTheodata::GetfCorrected(double *x, double *par) const{
  return par[0]*fy_correct[FindIndex(fx_correct, x[0])];
}

std::size_t KTheodata::FindIndex(const std::vector<double>& v, double val) const{
  for(auto x = v.begin(); x != v.end(); ++x){
    if(val <= *x + KUtil::LOOSE_EPSILON && val >= *x - KUtil::LOOSE_EPSILON){
      return std::distance(v.begin(), x);
    }
  }
  std::cout << "FindIndex: No such value(" << val << ") !!!" << std::endl;
  return 0;
}

void KTheodata::Clear(){
  fx.clear(); fy.clear(); fx_correct.clear(); fy_correct.clear();
}

std::size_t KTheodata::SetDataX(int nData, double *xdata){
  Clear();
  fx.resize(nData);
  for(std::size_t idx = 0; idx != nData; ++idx) fx.at(idx) = xdata[idx];
  return nData;
}

std::size_t KTheodata::SetDataY(int nData, double *ydata){
  Clear();
  fy.resize(nData);
  for(std::size_t idx = 0; idx != nData; ++idx) fy.at(idx) = ydata[idx];
  return nData;
}

std::size_t KTheodata::SetDataX(std::vector<double> &xdata){
  Clear();
  fx = xdata;
  return fx.size();;
}

std::size_t KTheodata::SetDataY(std::vector<double> &ydata){
  Clear();
  fy = ydata;
  return fy.size();;
}
