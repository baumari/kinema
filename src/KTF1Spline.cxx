#include <KTF1Spline.hh>
#include <vector>
#include <TSpline.h>

KTF1Spline::KTF1Spline(const char *name, KTheodata &Theo, bool flag)
{
  f = new TF1(name, Spline, Theo.GetfxMin(), Theo.GetfxMax(), Theo.GetN()*2+2);
  f->FixParameter(0, Theo.GetN());
  for(int i = 0; i != Theo.GetN(); ++i){
    f->FixParameter(i+1, Theo.fx[i]);
    f->FixParameter(i+1+Theo.GetN(), Theo.fy[i]);
  }
  if(!flag) f->FixParameter(2*Theo.GetN()+1, 1);
  else f->SetParameter(2*Theo.GetN()+1, 1);
}

KTF1Spline::~KTF1Spline(){delete f;}

double KTF1Spline::Spline(double *x, double *par){
  double xx = x[0];
  int nData = par[0];
  std::vector<double> xn, yn;
  xn.resize(nData); yn.resize(nData);
  for(int idx = 0; idx != nData; ++idx){
    xn[idx] = par[idx+1];
    yn[idx] = par[idx+1+nData];
  }
  TSpline3 sp3("sp3", &xn[0], &yn[0], nData);
  return sp3.Eval(xx)*par[2*nData+1];  
}
