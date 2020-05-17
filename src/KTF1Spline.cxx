#include <KTF1Spline.hh>

KTF1Spline::KTF1Spline(const char *name, double(*func)(double *, double *), KTheodata &Theo)
{
  f = new TF1(name, func, Theo.GetfxMin(), Theo.GetfxMax(), Theo.GetN()*2+2);
  f->FixParameter(0, Theo.GetN());
  for(int i = 0; i != Theo.GetN(); ++i){
    f->FixParameter(i+1, Theo.fx[i]);
    f->FixParameter(i+1+Theo.GetN(), Theo.fy[i]);
  }
}

KTF1Spline::~KTF1Spline(){delete f;}
