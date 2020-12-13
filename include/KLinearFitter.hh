#ifndef K_LINEARFITTER_HH
#define K_LINEARFITTER_HH

#include <TF1.h>
#include <vector>
// lienar fitting using nrutil. minimal implemented.
class KLinearFitter
{
private:
  double m_Chisquare;
  int m_nData;
  int m_nTotalFit;
  int  m_iMinChisq; // index of minimum chisqaure combination
  double *m_DataX, *m_DataY, *m_DataErr;
  std::vector<double> m_Coeff;
  std::vector<double> m_CoeffMin, m_CoeffMax; // limitation of parameters
  std::vector<double> m_CoeffErrMin, m_CoeffErrMax;
  // error of coefficient estimated by chisq-contour
  bool m_Svd;
  bool m_Error; 
  bool m_IsConditionChange;
  std::vector<bool> m_SetParLimits;
  std::vector<std::vector<double> > m_u, m_v;
  std::vector<double> m_w;
  std::vector<std::vector<double> > m_cvm;
  std::vector<double> m_dev;
  std::vector<TF1*> m_Func; // list of input function
  std::vector<TF1*> m_Fitfunc; // list of function actually used in fitting

  static void function(double, double *, std::vector<TF1*>&); //used by svdfit

public:
  KLinearFitter()
    : m_Chisquare(0), m_nData(0), m_nTotalFit(1), m_iMinChisq(1), 
      m_DataX(nullptr), m_DataY(nullptr), m_DataErr(nullptr),
      m_Svd(false), m_IsConditionChange(false) {}
  KLinearFitter(int nData, double *x, double *y, double *err)
    : m_Chisquare(0), m_nData(nData), m_nTotalFit(1), m_iMinChisq(1),
      m_DataX(x), m_DataY(y), m_DataErr(err),
      m_Svd(false), m_IsConditionChange(false) {}
  ~KLinearFitter() {}

public:
  void SetData(int nData, double *x, double *y, double *err); // experimental data
  void SetParLimits(int ipar, double min, double max);
  inline double GetChisquare() {return m_Chisquare;}
  inline double GetParameter(int ipar) {return m_Coeff.at(ipar);}
  inline std::vector<double> GetParameters() {return m_Coeff;}
  inline std::vector<std::vector<double> > GetCVM() {return m_cvm;}
  inline std::vector<double> GetDev() {return m_dev;}
  inline double GetDev(int ipar) {return m_dev[ipar];}
  inline double GetParError(int ipar) {return sqrt(m_dev.at(ipar));}
  std::vector<double> GetParErrors();
  inline int GetNDF() {return m_nData-m_Coeff.size()+1;}
  inline double GetReducedChisquare()
  {return m_Chisquare/((double)m_nData-(double)m_Coeff.size()+1.);}
  void Fit(const char *method = "svd");
  void AddFunction(TF1*); // add base function
  void AddFunction(std::vector<TF1*>&); // add list of base function
  void SetFuncList(std::vector<TF1*>&); // set list of base function  
  void ReleaseParameter(int ipar);
  void ErrorEstimationByChisquare(); // error estimation by chisq-contour

private:
  bool CheckParRange();
  void ChooseFitfunc(int ipar); // select function actually used in fitting
  void MakeCoefficient(int ifit); // make coefficent vector
  void MakeDeviation(int ifit); 
};

#endif



