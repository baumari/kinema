#include <KTF1Spline.hh>
#include <TSpline.h>
#include <algorithm>
#include <iostream>

KTF1Spline::KTF1Spline() : f(nullptr) {}

KTF1Spline::KTF1Spline(const KTF1Spline&obj)
{
  if(!obj.Getf()) f = nullptr;
  else f = (TF1*)(obj.Getf()->Clone());
}

KTF1Spline::KTF1Spline(const char *name, const KTheodata &Theo, bool flag)
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

KTF1Spline::KTF1Spline(const char *name, const std::vector<double>& vx,
		       const std::vector<double>& vy, bool flag)
{
  f = new TF1(name, Spline,
	      *std::min_element(vx.begin(), vx.end()),
	      *std::max_element(vx.begin(), vx.end()), vx.size()*2+2);
  f->FixParameter(0, vx.size());
  for(std::size_t i = 0; i != vx.size(); ++i){
    f->FixParameter(i+1, vx[i]);
    f->FixParameter(i+1+vx.size(), vy[i]);
  }
  if(!flag) f->FixParameter(2*vx.size()+1, 1);
  else f->SetParameter(2*vx.size()+1, 1);
}

KTF1Spline::KTF1Spline(const char *name, const int n,
		       const double *x,
		       const double *y,
		       bool flag)
{
  f = new TF1(name, Spline, x[0], x[n-1], n*2+2);
  f->FixParameter(0, n);
  for(int i = 0; i != n; ++i){
    f->FixParameter(i+1, x[i]);
    f->FixParameter(i+1+n, y[i]);
  }
  if(!flag) f->FixParameter(2*n+1, 1);
  else f->SetParameter(2*n+1, 1);
}

void KTF1Spline::SetData(const char *name, const KTheodata &Theo, bool flag)
{
  if(f){
    delete f;
    f = nullptr;
  }
  f = new TF1(name, Spline, Theo.GetfxMin(), Theo.GetfxMax(), Theo.GetN()*2+2);
  f->FixParameter(0, Theo.GetN());
  for(int i = 0; i != Theo.GetN(); ++i){
    f->FixParameter(i+1, Theo.fx[i]);
    f->FixParameter(i+1+Theo.GetN(), Theo.fy[i]);
  }
  if(!flag) f->FixParameter(2*Theo.GetN()+1, 1);
  else f->SetParameter(2*Theo.GetN()+1, 1);  
}

void KTF1Spline::SetData(const char *name, const std::vector<double>& vx,
			 const std::vector<double>& vy, bool flag)
{
  if(f){
    delete f;
    f = nullptr;
  }
  f = new TF1(name, Spline,
	      *std::min_element(vx.begin(), vx.end()),
	      *std::max_element(vx.begin(), vx.end()), vx.size()*2+2);
  f->FixParameter(0, vx.size());
  for(std::size_t i = 0; i != vx.size(); ++i){
    f->FixParameter(i+1, vx[i]);
    f->FixParameter(i+1+vx.size(), vy[i]);
  }
  if(!flag) f->FixParameter(2*vx.size()+1, 1);
  else f->SetParameter(2*vx.size()+1, 1);  
}

void KTF1Spline::SetData(const char *name, const int n, const double *x, const double *y,
			 bool flag)
{
  if(f){
    delete f;
    f = nullptr;
  }
  f = new TF1(name, Spline, x[0], x[n-1], n*2+2);
  f->FixParameter(0, n);
  for(int i = 0; i != n; ++i){
    f->FixParameter(i+1, x[i]);
    f->FixParameter(i+1+n, y[i]);
  }
  if(!flag) f->FixParameter(2*n+1, 1);
  else f->SetParameter(2*n+1, 1);  
}

KTF1Spline::~KTF1Spline()
{
  if(f) delete f;
}

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

void KTF1Spline::Clear()
{
  if(f){
    delete f;
    f = nullptr;
  }
}

TF1* KTF1Spline::Getf() const
{
  return f;
}
