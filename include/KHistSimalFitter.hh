#ifndef K_HISTSIMALFITTER_HH
#define K_HISTSIMALFITTER_HH

#include <TH1D.h>
#include <TF1.h>
#include <vector>

class KHistSimalFitter{
private:
  int fnHist;
  int fnFunc;
  TH1D** fHistList;
  TF1 *fFunction; // user function
  TF1 *fFitFunction; // fitting function 
  TH1D *fFitHist;
  double *fParameters;
  double *fParErrors;
  TFitResultPtr fFitResult;
  TF1 ***fFitResultFunction; // list of fitted function

public:
  KHistSimalFitter();
  KHistSimalFitter(int nHist, const TH1D** histlist,
		   const TF1* fitfunction);
  KHistSimalFitter(std::vector<TH1D*>& vHistList,
		   const TF1* fitfunction);  
  ~KHistSimalFitter();

  void SetHistList(int nHist, const TH1D** histlist);
  void ClearHist(); // clear all hist
  void SetFitFunction(const TF1* fitfun);
  TFitResultPtr Fit();
  TH1D* GetFitHist() const {return fFitHist;}
  TF1* GetFitFunction() const {return fFitFunction;}

  double* GetParameters() const
  {return fParameters;}
  double GetParameter(int ipar) const;
  double* GetParErrors() const
  {return fParErrors;}
  double GetParError(int ipar) const;
  TF1*** GetFitFunctionListAll() const
  {return fFitResultFunction;}
  TF1** GetFitFunctionList(int ihist) const;
  
private:
  TH1D* MakeFitHist();
  TF1* MakeFitFunction();
  void MakeFunctionList();
  //  Parse(); // should be implemented in TF1
};

#endif
