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
  std::vector<std::vector<double> > m_TheoY;
  std::vector<double> m_DataYOrg; // original data for experimental values
  std::vector<double> m_Coeff;
  std::vector<double> m_CoeffMin, m_CoeffMax; // limitation of parameters
  std::vector<double> m_CoeffErrMin, m_CoeffErrMax;
  // error of coefficient estimated by chisq-contour
  bool m_Svd;
  bool m_Error; 
  bool m_IsConditionChange;
  std::vector<bool> m_SetParLimits;
  int m_FixParameter; // bit mask 
  std::vector<std::vector<double> > m_u, m_v;
  std::vector<double> m_w;
  std::vector<std::vector<double> > m_cvm;
  std::vector<double> m_dev;
  std::vector<double> m_a;
  std::vector<TF1*> m_Func; // list of input function
  std::vector<std::vector<double> > m_Fitfunc; // list of function actually used in fitting
  std::vector<std::vector<double> > m_ChisqLog, m_CoeffLog;
  static void function(int, double *,
		       std::vector<std::vector<double> >&); //used by svdfit

public:
  KLinearFitter()
    : m_Chisquare(0), m_nData(0), m_nTotalFit(1), m_iMinChisq(1), 
      m_DataX(nullptr), m_DataY(nullptr), m_DataErr(nullptr), m_FixParameter(0), 
      m_Svd(false), m_IsConditionChange(false) {}
  KLinearFitter(int nData, double *x, double *y, double *err)
    : m_Chisquare(0), m_nData(nData), m_nTotalFit(1), m_iMinChisq(1),
      m_DataX(x), m_DataY(y), m_DataErr(err), m_FixParameter(0), 
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
  inline double GetParError(int ipar) {return sqrt(m_dev.at(ipar));} // cvm error
  inline std::size_t GetNPar() {return m_Func.size();}
  std::vector<double> GetParErrors(); // cvm error
  inline int GetNDF() {return m_nData - m_Fitfunc.size() - 1;} // return actual ndof related to minimum chisquare
  inline double GetReducedChisquare() {return GetChisquare()/GetNDF();}
  void Fit(const char *method = "svd");
  void AddFunction(TF1*); // add base function
  void AddFunction(std::vector<TF1*>&); // add list of base function
  void SetFuncList(std::vector<TF1*>&); // set list of base function  
  void ReleaseParameter(int ipar);
  void FixParameter(int ipar, double val);
  int GetNParFixed();
  inline double GetParErrorMin(int ipar) {return m_CoeffErrMin[ipar];}
  inline double GetParErrorMax(int ipar) {return m_CoeffErrMax[ipar];}  
  void ErrorEstimationByChisquare(); // error estimation by chisq-contour
  inline std::vector<double> GetChisqLog(int ipar) {return m_ChisqLog[ipar];}
  inline std::vector<double> GetCoeffLog(int ipar) {return m_CoeffLog[ipar];}  
 
private:
  bool CheckParRange();
  void ChooseFitfunc(int ipar); // select function actually used in fitting
  void MakeCoefficient(int ifit); // make coefficent vector
  void MakeDeviation(int ifit);
  void MakeTheoData(); // make theoretical Y used in fittig routine
  void MakeExpData(); // correction for exp data (fixed, release parameter)
};

#endif



