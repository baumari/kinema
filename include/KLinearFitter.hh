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
  bool m_NoSolution;
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
      m_Svd(false), m_IsConditionChange(false), m_NoSolution(false) {}
  KLinearFitter(int nData, const double *x, const double *y, const double *err)
    : m_Chisquare(0), m_nTotalFit(1), m_iMinChisq(1),
      m_FixParameter(0), 
      m_Svd(false), m_IsConditionChange(false), m_NoSolution(false)
  {
    if(!x || !y || !err){
      m_nData = 0;
    }else m_nData = nData;
    m_DataX = new double[nData];
    m_DataY = new double[nData];
    m_DataErr = new double[nData];
    for(int i = 0; i != nData; ++i){
      m_DataX[i] = x[i];
      m_DataY[i] = y[i];
      m_DataErr[i] = err[i];
    }
  }
  ~KLinearFitter()
  {
    if(m_DataX) delete[] m_DataX;
    if(m_DataY) delete[] m_DataY;
    if(m_DataErr) delete[] m_DataErr;
  }

public:
  void SetData(int nData, const double *x, const double *y, const double *err); // experimental data
  void SetParLimits(int ipar, double min, double max);
  inline double GetChisquare() const {return m_Chisquare;}
  inline double GetParameter(int ipar) const {return m_Coeff.at(ipar);}
  inline std::vector<double> GetParameters() const {return m_Coeff;}
  inline std::vector<std::vector<double> > GetCVM() const {return m_cvm;}
  inline std::vector<double> GetDev() const {return m_dev;}
  inline double GetDev(int ipar) const {return m_dev[ipar];}
  inline double GetParError(int ipar) const {return sqrt(m_dev.at(ipar));} // cvm error
  inline std::size_t GetNPar() const {return m_Func.size();}
  std::vector<double> GetParErrors(); // cvm error
  inline int GetNDF() const {return m_nData - m_Func.size() - 1;} // return actual ndof related to minimum chisquare
  inline double GetReducedChisquare() const {return GetChisquare()/GetNDF();}
  inline bool NonSolution() const {return m_NoSolution;}
  void Fit(const char *method = "svd");
  void AddFunction(TF1*); // add base function
  void AddFunction(std::vector<TF1*>&); // add list of base function
  void SetFuncList(std::vector<TF1*>&); // set list of base function  
  void ReleaseParameter(int ipar);
  void FixParameter(int ipar, double val);
  int GetNParFixed();
  inline double GetParErrorMin(int ipar) const {return m_CoeffErrMin[ipar];}
  inline double GetParErrorMax(int ipar) const {return m_CoeffErrMax[ipar];}  
  void ErrorEstimationByChisquare(); // error estimation by chisq-contour
  inline std::vector<double> GetChisqLog(int ipar) const {return m_ChisqLog[ipar];}
  inline std::vector<double> GetCoeffLog(int ipar) const {return m_CoeffLog[ipar];}
  void Clear(); // forget all data
 
private:
  bool CheckParRange(int ifit);
  void ChooseFitfunc(int ipar); // select function actually used in fitting
  void MakeCoefficient(int ifit); // make coefficent vector
  void MakeDeviation(int ifit);
  void MakeTheoData(); // make theoretical Y used in fittig routine
  void MakeExpData(); // correction for exp data (fixed, release parameter)
  double GetChisquareForce();
};

#endif



