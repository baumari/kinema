#ifndef K_NRUTIL_HH
#define K_NRUTIL_HH

#include <vector>
#include <TF1.h>

static float sqrarg;
#define SQR(a) ((sqrarg=(a)) == 0.0 ? 0.0 : sqrarg*sqrarg)

static double dsqrarg;
#define DSQR(a) ((dsqrarg=(a)) == 0.0 ? 0.0 : dsqrarg*dsqrarg)

static double dmaxarg1, dmaxarg2;
#define DMAX(a, b) (dmaxarg1=(a), dmaxarg2=(b), (dmaxarg1) > (dmaxarg2) ?\
(dmaxarg1) : (dmaxarg2))

static double dminarg1, dminarg2;
#define DMIN(a, b) (dminarg1=(a), dminarg2=(b), (dminarg1) < (dminarg2) ?\
(dminarg1) : (dminarg2))

static float maxarg1, maxarg2;
#define FMAX(a, b) (maxarg1=(a), maxarg2=(b), (maxarg1) > (maxarg2) ?\
(maxarg1) : (maxarg2))

static float minarg1, minarg2;
#define FMIN(a, b) (minarg1=(a), minarg2=(b), (minarg1) < (minarg2) ?\
(minarg1) : (minarg2))

static long lmaxarg1, lmaxarg2;
#define LMAX(a, b) (lmaxarg1=(a), lmaxarg2=(b), (lmaxarg1) > (lmaxarg2) ?\
(lmaxarg1) : (lmaxarg2))

static long lminarg1, lminarg2;
#define LMIN(a, b) (lminarg1=(a), lminarg2=(b), (lminarg1) < (lminarg2) ?\
(lminarg1) : (lminarg2))

static int imaxarg1, imaxarg2;
#define IMAX(a, b) (imaxarg1=(a), imaxarg2=(b), (imaxarg1) > (imaxarg2) ?\
(imaxarg1) : (imaxarg2))

static int iminarg1, iminarg2;
#define IMIN(a, b) (iminarg1=(a), iminarg2=(b), (iminarg1) < (iminarg2) ?\
(iminarg1) : (iminarg2))

#define SIGN(a, b) ((b) >= 0.0 ? fabs(a) : -fabs(a))

#define NINT(a) ((int) ((a) + ((a) > 0. ? (.5) : (-.5))))


// import from numerical recipes in C

namespace KNrutil{
  void nrerror(char error_text[]);
  void svbksb(std::vector<std::vector<double> > &u,
	      std::vector<double>& w,
	      std::vector<std::vector<double> >& v, int m, int n,
	      double b[], double x[]);
  void svdcmp(std::vector<std::vector<double> >&a, int m, int n, std::vector<double>& w,
	      std::vector<std::vector<double> >& v);
  void svdvar(std::vector<std::vector<double> >& v,int ma, std::vector<double>& w,
	      std::vector<std::vector<double> >& cvm);
  void svdfit(double y[], double sig[], int ndata,
	      double a[], int ma,
	      std::vector<std::vector<double> >& u,
	      std::vector<std::vector<double> >& v,
	      std::vector<double>& w, std::vector<std::vector<double> >& funclist, 
	      double *chisq,
	      void(*funcs)(int, double [], std::vector<std::vector<double> >&));
  double pythag(double, double);

  
}; // namespace KNrutil
#endif
