#include <KHistSimalFitter.hh>

KHistSimalFitter::KHistSimalFitter() :
    fnHist(0),
    fnFunc(0),
    fHistList(nullptr),
    fFunction(nullptr),
    fFitFunction(nullptr),
    fFitHist(nullptr),
    fParameters(nullptr),
    fParErrors(nullptr),
    fFitResult(nullptr),
    fFitResultFunction(nullptr) {}
  

KHistSimalFitter::KHistSimalFitter(int nHist,
				   const TH1D** histlist,
				   const TF1* fitfunc) :
  fnFunc(0),
  fFitFunction(nullptr),
  fFitHist(nullptr),
  fParameters(nullptr),
  fParErrors(nullptr),
  fFitResult(nullptr),
  fFitResultFunction(nullptr)
{
  fHistList = new TH1D*[nHist];
  for(int i = 0; i != nHist; ++i){
    fHistList[i] = (TH1D*)histlist[i]->Clone();
  }
  fFunction = (TF1*)fitfunc->Clone();
  fnHist = nHist;
}

KHistSimalFitter::KHistSimalFitter(std::vector<TH1D*>& vHistList,
				   const TF1* fitfunc) :
  fnFunc(0),
  fFitFunction(nullptr),
  fFitHist(nullptr),
  fParameters(nullptr),
  fParErrors(nullptr),
  fFitResult(nullptr),
  fFitResultFunction(nullptr)
{
  fHistList = new TH1D*[vHistList.size()];
  for(int i = 0; i != vHistList.size(); ++i){
    fHistList[i] = (TH1D*)vHistList[i]->Clone();
  }
  fFunction = (TF1*)fitfunc->Clone();
  fnHist = (int)vHistList.size();
}

KHistSimalFitter::~KHistSimalFitter(){
  if(fHistList){
    for(int i = 0; i != fnHist; ++i) if(fHistList[i])
				       delete fHistList[i];
    delete [] fHistList;
  }
  if(fFunction) delete fFunction;
  // if(fFitResultFunction){
  //   for(int i = 0; i != fnHist; ++i){
  //     if(fFitResultFunction[i]){
  // 	for(int j = 0; j != fnFunc; ++j)
  // 	  delete fFitResultFunction[i][j];
  // 	delete [] fFitResultFunction[i];
  //     }
  //   }
  //   delete [] fFitResultFunction;
  // }  
}

void KHistSimalFitter::SetHistList(int nHist,
				   const TH1D** histlist){
  if(fHistList) ClearHist();
  fHistList = new TH1D*[nHist];
  for(int i = 0; i != nHist; ++i){
    fHistList[i] = (TH1D*)histlist[i]->Clone();
  }
  fnHist = nHist;
}

void KHistSimalFitter::ClearHist(){
  if(fHistList){
    for(int i = 0; i != fnHist; ++i) if(fHistList[i])
				       delete fHistList[i];
    delete [] fHistList;
  }
  fHistList = nullptr;
}

void KHistSimalFitter::SetFitFunction(const TF1* fitfun){
  if(fFunction) delete fFunction;
  fFunction = (TF1*)fitfun->Clone();
}

TFitResultPtr KHistSimalFitter::Fit(){
  if(!fHistList){
    std::cout << "[KHistSimalFitter::Fit] No histograms to be fitted!!!" << std::endl;
    return nullptr;
  }
  if(!fFunction){
    std::cout
      << "[KHistSimalFitter::Fit] No fitting function!!!" << std::endl;
    return nullptr;    
  }
  fFitHist = MakeFitHist();
  fFitFunction = MakeFitFunction();
  fFitResult = fFitHist->Fit(fFitFunction, "S","",
			     fFitHist->GetXaxis()->GetXmin(),
			     fFitHist->GetXaxis()->GetXmax());
  fParameters = (double*)fFitFunction->GetParameters();
  fParErrors = (double*)fFitFunction->GetParErrors();
  MakeFunctionList();
  return fFitResult;
}

double KHistSimalFitter::GetParameter(int ipar) const {
  if(!fParameters) return 0;
  return fParameters[ipar];
}

double KHistSimalFitter::GetParError(int ipar) const {
  if(!fParErrors) return 0;
  return fParErrors[ipar];
}

TF1** KHistSimalFitter::GetFitFunctionList(int ihist) const {
  if(ihist >= fnHist) return nullptr;
  return fFitResultFunction[ihist];
}

TH1D* KHistSimalFitter::MakeFitHist(){
  auto fxmin = fFunction->GetXmin();
  auto fxmax = fFunction->GetXmax();
  auto width = fHistList[0]->GetBinWidth(1);
  auto minbin = fHistList[0]->FindFixBin(fxmin);
  auto maxbin = fHistList[0]->FindFixBin(fxmax);
  auto xmin =
    fHistList[0]->GetBinCenter(minbin) - width/2.;
  auto xmax =
    fHistList[0]->GetBinCenter(maxbin) + width/2.;      
  auto nBinPerHist = maxbin - minbin + 1;
  auto h = new TH1D("h","h",
		    (2*fnHist-1)*nBinPerHist,
		    xmin,
		    xmin + (2.*fnHist-1.)*(xmax-xmin));
  for(int ihist = 0; ihist != fnHist; ++ihist){
    for(int ibin = 1; ibin != nBinPerHist+1; ++ibin){
      h->SetBinContent(2*ihist*nBinPerHist + ibin,
		       fHistList[ihist]
		       ->GetBinContent(ibin+minbin-1));
      h->SetBinError(2*ihist*nBinPerHist + ibin,
		     fHistList[ihist]
		     ->GetBinError(ibin+minbin-1));
    }
  }
  return h;
}

TF1* KHistSimalFitter::MakeFitFunction(){
  // temp inplement ...
  double fxmin = fFunction->GetXmin();
  double fxmax = fFunction->GetXmax();
  auto width = fFitHist->GetBinWidth(1);
  auto offset = fxmax-fxmin+width;
  auto f = new TF1("f",[&](double *x, double *p)
  {
    if(x[0] >= 10.4 && x[0] <= 13.1){
      return p[1]*TMath::Voigt(x[0]-p[2]-p[0],7.78106e-02,p[4]) + // h1501 in 10000.root
	p[5]*TMath::Voigt(x[0]-p[6]-p[0],7.78106e-02,p[8]) +
	p[9]*TMath::Voigt(x[0]-p[10]-p[0],7.78106e-02,p[12]) +
	p[13]*TMath::Voigt(x[0]-p[14]-p[0],7.78106e-02,p[16]) +
	p[17]*TMath::Voigt(x[0]-p[18]-p[0],7.78106e-02,p[20]) +
	p[21]*TMath::Voigt(x[0]-p[22]-p[0],7.78106e-02,p[24]) +
	p[49]*TMath::Voigt(x[0]-p[50]-p[0],7.78106e-02,p[52]) +
	p[53]*TMath::Voigt(x[0]-p[54]-p[0],7.78106e-02,p[56]) +
	p[57]*TMath::Voigt(x[0]-p[58]-p[0],7.78106e-02,p[60]) +
	p[61]*TMath::Voigt(x[0]-p[62]-p[0],7.78106e-02,p[64]) +
	p[65]*TMath::Voigt(x[0]-p[66]-p[0],7.78106e-02,p[68]) +
	p[69]+p[70]*x[0]+p[71]*pow(x[0],2)+p[72]*pow(x[0],3)  +
	p[153]*TMath::Voigt(x[0]-p[154]-p[0],7.78106e-02,p[155]);
    }else if(x[0] >= 10.4 + 2.*2.72 && x[0] <= 13.1 + 2.*2.72){
      return p[73]*TMath::Voigt(x[0]-2.*2.72-p[2]-p[148],8.19648e-02,p[4]) + // h1504 in 10000.root
  	p[74]*TMath::Voigt(x[0]-2.*2.72-p[6]-p[148],8.19648e-02,p[8]) +
  	p[75]*TMath::Voigt(x[0]-2.*2.72-p[10]-p[148],8.19648e-02,p[12]) +
  	p[76]*TMath::Voigt(x[0]-2.*2.72-p[14]-p[148],8.19648e-02,p[16]) +
  	p[77]*TMath::Voigt(x[0]-2.*2.72-p[18]-p[148],8.19648e-02,p[20]) +
  	p[78]*TMath::Voigt(x[0]-2.*2.72-p[22]-p[148],8.19648e-02,p[24]) +
  	p[79]*TMath::Voigt(x[0]-2.*2.72-p[50]-p[148],8.19648e-02,p[52]) +
  	p[80]*TMath::Voigt(x[0]-2.*2.72-p[54]-p[148],8.19648e-02,p[56]) +
  	p[81]*TMath::Voigt(x[0]-2.*2.72-p[58]-p[148],8.19648e-02,p[60]) +
  	p[82]*TMath::Voigt(x[0]-2.*2.72-p[62]-p[148],8.19648e-02,p[64]) +
  	p[83]*TMath::Voigt(x[0]-2.*2.72-p[66]-p[148],8.19648e-02,p[68]) +
	p[84]+p[85]*x[0]+p[86]*pow(x[0],2)+p[87]*pow(x[0],3) +
	p[156]*TMath::Voigt(x[0]-2.*2.72-p[154]-p[148],8.19648e-02,p[155]);
    }else if(x[0] >= 10.4 + 4.*2.72 && x[0] <= 13.1 + 4.*2.72){
      return p[88]*TMath::Voigt(x[0]-4.*2.72-p[2]-p[149],6.21e-02,p[4]) + // h1501 in 10025.root
	p[89]*TMath::Voigt(x[0]-4.*2.72-p[6]-p[149],6.21e-02,p[8]) +
	p[90]*TMath::Voigt(x[0]-4.*2.72-p[10]-p[149],6.21e-02,p[12]) +
	p[91]*TMath::Voigt(x[0]-4.*2.72-p[14]-p[149],6.21e-02,p[16]) +
	p[92]*TMath::Voigt(x[0]-4.*2.72-p[18]-p[149],6.21e-02,p[20]) +
	p[93]*TMath::Voigt(x[0]-4.*2.72-p[22]-p[149],6.21e-02,p[24]) +
	p[94]*TMath::Voigt(x[0]-4.*2.72-p[50]-p[149],6.21e-02,p[52]) +
	p[95]*TMath::Voigt(x[0]-4.*2.72-p[54]-p[149],6.21e-02,p[56]) +
	p[96]*TMath::Voigt(x[0]-4.*2.72-p[58]-p[149],6.21e-02,p[60]) +
  	p[97]*TMath::Voigt(x[0]-4.*2.72-p[62]-p[149],6.21e-02,p[64]) +
  	p[98]*TMath::Voigt(x[0]-4.*2.72-p[66]-p[149],6.21e-02,p[68]) +
  	p[99]+p[100]*x[0]+p[101]*pow(x[0],2)+p[102]*pow(x[0],3) +
  	p[157]*TMath::Voigt(x[0]-4.*2.72-p[154]-p[149],6.21e-02,p[155]);	
  }else if(x[0] >= 10.4 + 6.*2.72 && x[0] <= 13.1 + 6.*2.72){
      return p[103]*TMath::Voigt(x[0]-6.*2.72-p[2]-p[150],5.74004e-02,p[4]) + // h1502 in 10025.root
  	p[104]*TMath::Voigt(x[0]-6.*2.72-p[6]-p[150],5.74004e-02,p[8]) +
  	p[105]*TMath::Voigt(x[0]-6.*2.72-p[10]-p[150],5.74004e-02,p[12]) +
  	p[106]*TMath::Voigt(x[0]-6.*2.72-p[14]-p[150],5.74004e-02,p[16]) +
  	p[107]*TMath::Voigt(x[0]-6.*2.72-p[18]-p[150],5.74004e-02,p[20]) +
  	p[108]*TMath::Voigt(x[0]-6.*2.72-p[22]-p[150],5.74004e-02,p[24]) +
  	p[109]*TMath::Voigt(x[0]-6.*2.72-p[50]-p[150],5.74004e-02,p[52]) +
  	p[110]*TMath::Voigt(x[0]-6.*2.72-p[54]-p[150],5.74004e-02,p[56]) +
  	p[111]*TMath::Voigt(x[0]-6.*2.72-p[58]-p[150],5.74004e-02,p[60]) +
  	p[112]*TMath::Voigt(x[0]-6.*2.72-p[62]-p[150],5.74004e-02,p[64]) +
  	p[113]*TMath::Voigt(x[0]-6.*2.72-p[66]-p[150],5.74004e-02,p[68]) +
  	p[114]+p[115]*x[0]+p[116]*pow(x[0],2)+p[117]*pow(x[0],3) +
	p[158]*TMath::Voigt(x[0]-6.*2.72-p[154]-p[150],5.74004e-02,p[155]);	      
  }else if(x[0] >= 10.4 + 8.*2.72 && x[0] <= 13.1 + 8.*2.72){
      return p[118]*TMath::Voigt(x[0]-8.*2.72-p[2]-p[151],5.34052e-02,p[4]) + // h1503 in 10025.root
  	p[119]*TMath::Voigt(x[0]-8.*2.72-p[6]-p[151],5.34052e-02,p[8]) +
  	p[120]*TMath::Voigt(x[0]-8.*2.72-p[10]-p[151],5.34052e-02,p[12]) +
  	p[121]*TMath::Voigt(x[0]-8.*2.72-p[14]-p[151],5.34052e-02,p[16]) +
  	p[122]*TMath::Voigt(x[0]-8.*2.72-p[18]-p[151],5.34052e-02,p[20]) +
  	p[123]*TMath::Voigt(x[0]-8.*2.72-p[22]-p[151],5.34052e-02,p[24]) +
  	p[124]*TMath::Voigt(x[0]-8.*2.72-p[50]-p[151],5.34052e-02,p[52]) +
  	p[125]*TMath::Voigt(x[0]-8.*2.72-p[54]-p[151],5.34052e-02,p[56]) +
  	p[126]*TMath::Voigt(x[0]-8.*2.72-p[58]-p[151],5.34052e-02,p[60]) +
  	p[127]*TMath::Voigt(x[0]-8.*2.72-p[62]-p[151],5.34052e-02,p[64]) +
  	p[128]*TMath::Voigt(x[0]-8.*2.72-p[66]-p[151],5.34052e-02,p[68]) +
  	p[129]+p[130]*x[0]+p[131]*pow(x[0],2)+p[132]*pow(x[0],3) +
	p[159]*TMath::Voigt(x[0]-8.*2.72-p[154]-p[151],5.34052e-02,p[155]);    
    }else if(x[0] >= 10.4 + 10.*2.72 && x[0] <= 13.1 + 10.*2.72){
      return p[133]*TMath::Voigt(x[0]-10.*2.72-p[2]-p[152],5.1219e-02,p[4]) + // h1504 in 10025.root
  	p[134]*TMath::Voigt(x[0]-10.*2.72-p[6]-p[152],5.1219e-02,p[8]) +
  	p[135]*TMath::Voigt(x[0]-10.*2.72-p[10]-p[152],5.1219e-02,p[12]) +
  	p[136]*TMath::Voigt(x[0]-10.*2.72-p[14]-p[152],5.1219e-02,p[16]) +
  	p[137]*TMath::Voigt(x[0]-10.*2.72-p[18]-p[152],5.1219e-02,p[20]) +
  	p[138]*TMath::Voigt(x[0]-10.*2.72-p[22]-p[152],5.1219e-02,p[24]) +
  	p[139]*TMath::Voigt(x[0]-10.*2.72-p[50]-p[152],5.1219e-02,p[52]) +
  	p[140]*TMath::Voigt(x[0]-10.*2.72-p[54]-p[152],5.1219e-02,p[56]) +
  	p[141]*TMath::Voigt(x[0]-10.*2.72-p[58]-p[152],5.1219e-02,p[60]) +
  	p[142]*TMath::Voigt(x[0]-10.*2.72-p[62]-p[152],5.1219e-02,p[64]) +
  	p[143]*TMath::Voigt(x[0]-10.*2.72-p[66]-p[152],5.1219e-02,p[68]) +
  	p[144]+p[145]*x[0]+p[146]*pow(x[0],2)+p[147]*pow(x[0],3) +
  	p[160]*TMath::Voigt(x[0]-10.*2.72-p[154]-p[152],5.1219e-02,p[155]);      
    }else{
      return 0.;
    }
  },
    fFitHist->GetXaxis()->GetXmin(),
    fFitHist->GetXaxis()->GetXmax(), 161);
  //  double sigma = 0;
  f->SetParLimits(1, 0., 1000);
  f->FixParameter(2, 10.753);
  //  f->FixParameter(3, sigma);
  f->FixParameter(4, 0.055);      
  
  f->SetParLimits(5, 0.,1000);
  f->FixParameter(6, 10.818);
  //  f->FixParameter(7, sigma);
  f->FixParameter(8, 0.024); 
  
  f->SetParLimits(9, 0.,1000);
  f->FixParameter(10, 10.996);
  //  f->FixParameter(11, sigma);
  f->FixParameter(12, 0.037);      
  
  f->SetParLimits(13, 0.,10000);
  f->FixParameter(14, 11.08);
  //  f->FixParameter(15, sigma);
  f->FixParameter(16, 0.004);

  f->SetParLimits(17, 0.,10000);
  f->FixParameter(18, 11.748);
  //  f->FixParameter(19, sigma);
  f->FixParameter(20, 0.110);

  f->SetParLimits(21, 0.,10000);
  f->FixParameter(22, 11.848);
  //  f->FixParameter(23, sigma);
  f->FixParameter(24, 0.068);

  f->FixParameter(25, 0);  
  f->FixParameter(26, 11.950);
  //  f->FixParameter(27, sigma);
  f->FixParameter(28, 0.500);

  f->FixParameter(29, 0.);
  f->FixParameter(30, 12.106);
  //  f->FixParameter(31, sigma);
  f->FixParameter(32, 0.540);
  
  f->FixParameter(33, 0.);  
  f->FixParameter(34, 12.130);
  //  f->FixParameter(35, sigma);
  f->FixParameter(36, 0.080);

  f->FixParameter(37, 0.);
  f->FixParameter(38, 12.140);
  //  f->FixParameter(39, sigma);
  f->FixParameter(40, 0.430);
  
  f->FixParameter(41, 0.);
  f->FixParameter(42, 12.187);
  //  f->FixParameter(43, sigma);
  f->FixParameter(44, 0.150);

  f->FixParameter(45, 0.);
  f->FixParameter(46, 12.438);
  //  f->FixParameter(47, sigma);
  f->FixParameter(48, 0.140); // NNDC to here

  f->SetParLimits(49, 0.,10000);
  //  fit->FixParameter(50, 12.052);  
  f->SetParLimits(50, 12.052-0.02,12.052+0.02);  
  //  f->FixParameter(51, sigma);
  //  fit->FixParameter(52, 0.015372713);
  //  fit->FixParameter(52, 0.031);  
  f->SetParLimits(52, 0., 0.031+0.05);    

  f->SetParLimits(53, 0.,10000);
  //  fit->FixParameter(54, 12.253764);
  //  fit->FixParameter(54, 12.26);  
  f->SetParLimits(54, 12.26-0.02, 12.26+0.02);  
  //  f->FixParameter(55, sigma);
  //  fit->FixParameter(56, .01);
  f->SetParLimits(56, 0., .01+0.05);  
  
  f->SetParLimits(57, 0.,10000);
  //  fit->FixParameter(58, 12.428204);
  //  fit->FixParameter(58, 12.43);  
  f->SetParLimits(58, 12.43-0.02,12.43+0.02);  
  //  f->FixParameter(59, sigma);
  //  fit->FixParameter(60, 0.);  
  f->SetParLimits(60, 0., 0.02);
  
  f->SetParLimits(61, 0.,1000.);
  //  fit->FixParameter(61, 0.);  
  //  fit->FixParameter(62, 12.59);
  f->SetParLimits(62, 12.59-0.02,12.59+0.02);  
  //  f->FixParameter(63, sigma);
  //  fit->FixParameter(64, 0.);
  f->SetParLimits(64, 0.,0.02);  
  
  f->SetParLimits(65, 0.,1000.);
  //  fit->FixParameter(65, 0.);
  //  fit->FixParameter(66, 12.78);
  f->SetParLimits(66, 12.78-0.02,12.78+0.02);  
  //  f->FixParameter(67, sigma);
  //  fit->FixParameter(68, 0.);
  f->SetParLimits(68, 0., 0.02);  

  // back
  f->SetParameter(69, 0);
  f->SetParameter(70, 0);  
  f->SetParameter(71, 0);
  f->FixParameter(72, 0);

  for(int ipar = 73; ipar != 84; ++ipar) f->SetParLimits(ipar, 0., 1000.);
  f->SetParameter(84, 0);
  f->SetParameter(85, 0);  
  f->SetParameter(86, 0);
  f->FixParameter(87, 0);

  for(int ipar = 88; ipar != 99; ++ipar) f->SetParLimits(ipar, 0., 1000.);
  f->SetParameter(99, 0);
  f->SetParameter(100, 0);  
  f->SetParameter(101, 0);
  f->FixParameter(102, 0);

  for(int ipar = 103; ipar != 114; ++ipar) f->SetParLimits(ipar, 0., 1000.);
  f->SetParameter(114, 0);
  f->SetParameter(115, 0);  
  f->SetParameter(116, 0);
  f->FixParameter(117, 0);

  for(int ipar = 118; ipar != 129; ++ipar) f->SetParLimits(ipar, 0., 1000.);
  f->SetParameter(129, 0);
  f->SetParameter(130, 0);  
  f->SetParameter(131, 0);
  f->FixParameter(132, 0);

  for(int ipar = 133; ipar != 144; ++ipar) f->SetParLimits(ipar, 0., 1000.);
  f->SetParameter(144, 0);
  f->SetParameter(145, 0);  
  f->SetParameter(146, 0);
  f->FixParameter(147, 0);

  f->SetParLimits(0.,-0.05,0.05);
  f->SetParLimits(148.,-0.05,0.05);
  f->SetParLimits(149.,-0.05,0.05);
  f->SetParLimits(150.,-0.05,0.05);
  f->SetParLimits(151.,-0.05,0.05);
  f->SetParLimits(152.,-0.05,0.05);

  f->SetParLimits(154,11.6-0.05,11.6+0.05);
  f->SetParLimits(155,0.,0.5);
  f->SetParLimits(153,0.,1000);  
  f->SetParLimits(156,0.,1000);
  f->SetParLimits(157,0.,1000);
  f->SetParLimits(158,0.,1000);
  f->SetParLimits(159,0.,1000);
  f->SetParLimits(160,0.,1000);
  // f->FixParameter(153,0.);  
  // f->FixParameter(156,0.);
  // f->FixParameter(157,0.);
  // f->FixParameter(158,0.);
  // f->FixParameter(159,0.);
  // f->FixParameter(160,0.);  
  
  return f;
}

void KHistSimalFitter::MakeFunctionList(){
  fFitResultFunction = new TF1**[fnHist];
  for(int ihist = 0; ihist != fnHist; ++ihist){
    fFitResultFunction[ihist] = new TF1*[14];
  }
  // h1501 in 10000.root
  for(int ifunc = 0; ifunc != 11; ++ifunc){
    fFitResultFunction[0][ifunc]
      = new TF1(Form("f0_%d",ifunc),
		"[0]*TMath::Voigt(x-[1],[2],[3])",
		10.4,13.1);
  }
  for(int ifunc = 0; ifunc != 6; ++ifunc){
    fFitResultFunction[0][ifunc]->SetParameters(fFitFunction->GetParameter(4*ifunc+1),
						fFitFunction->GetParameter(4*ifunc+2)+fFitFunction->GetParameter(0),
						7.78106e-02,
						fFitFunction->GetParameter(4*ifunc+4));
  }
  for(int ifunc = 0; ifunc != 5; ++ifunc){
    fFitResultFunction[0][ifunc+6]->SetParameters(fFitFunction->GetParameter(4*ifunc+49),
						  fFitFunction->GetParameter(4*ifunc+50)+fFitFunction->GetParameter(0),
						  7.78106e-02,
						  fFitFunction->GetParameter(4*ifunc+52));
  }  
  fFitResultFunction[0][11] = new TF1("f0_11",
				      "[0]+[1]*x+[2]*pow(x,2)+[3]*pow(x,3)",10.4,13.1);
  fFitResultFunction[0][11]->SetParameters(fFitFunction->GetParameter(69),
					   fFitFunction->GetParameter(70),
					   fFitFunction->GetParameter(71),
					   fFitFunction->GetParameter(72));
  fFitResultFunction[0][12] = new TF1("f0_12",
				      "[0]*TMath::Voigt(x-[1],[2],[3])",
				      10.4,13.1);
  fFitResultFunction[0][12]->SetParameters(fFitFunction->GetParameter(153),
					   fFitFunction->GetParameter(154) + fFitFunction->GetParameter(0),
					   7.78106e-02,
					   fFitFunction->GetParameter(155));
  fFitResultFunction[0][13] = new TF1("f0_13",[=](double*x, double *p)
  {
    double val = 0;
    for(int ifunc = 0; ifunc != 13; ++ifunc){
      val+=fFitResultFunction[0][ifunc]->Eval(x[0]);
    }
    return val;
  },
    10.4, 13.1,0);
  // h1504 in 10000.root
  for(int ifunc = 0; ifunc != 11; ++ifunc){
    fFitResultFunction[1][ifunc]
      = new TF1(Form("f1_%d",ifunc),
		"[0]*TMath::Voigt(x-[1],[2],[3])",
		10.4,13.1);
  }
  for(int ifunc = 0; ifunc != 6; ++ifunc){
    fFitResultFunction[1][ifunc]->SetParameters(fFitFunction->GetParameter(73+ifunc),
						fFitFunction->GetParameter(4*ifunc+2)+fFitFunction->GetParameter(148),
						8.19648e-02,
						fFitFunction->GetParameter(4*ifunc+4));
  }
  for(int ifunc = 0; ifunc != 5; ++ifunc){
    fFitResultFunction[1][ifunc+6]->SetParameters(fFitFunction->GetParameter(79+ifunc),
						  fFitFunction->GetParameter(4*ifunc+50)+fFitFunction->GetParameter(148),
						  8.19648e-02,
						  fFitFunction->GetParameter(4*ifunc+52));
  }    
  fFitResultFunction[1][11] = new TF1("f1_11",
				      "[0]+[1]*(x+2.*2.72)+[2]*pow(x+2.*2.72,2)+[3]*pow(x+2.*2.72,3)",10.4,13.1);
  fFitResultFunction[1][11]->SetParameters(fFitFunction->GetParameter(84),
					   fFitFunction->GetParameter(85),
					   fFitFunction->GetParameter(86),
					   fFitFunction->GetParameter(87));
  fFitResultFunction[1][12] = new TF1("f1_12",
				      "[0]*TMath::Voigt(x-[1],[2],[3])",
				      10.4,13.1);
  fFitResultFunction[1][12]->SetParameters(fFitFunction->GetParameter(156),
					   fFitFunction->GetParameter(154) + fFitFunction->GetParameter(148),
					   8.19648e-02,
					   fFitFunction->GetParameter(155));
  fFitResultFunction[1][13] = new TF1("f1_13",[=](double*x, double *p)
  {
    double val = 0;
    for(int ifunc = 0; ifunc != 13; ++ifunc){
      val+=fFitResultFunction[1][ifunc]->Eval(x[0]);
    }
    return val;
  },
    10.4, 13.1,0);
  
  // h1501 in 10025.root
  for(int ifunc = 0; ifunc != 11; ++ifunc){
    fFitResultFunction[2][ifunc]
      = new TF1(Form("f2_%d",ifunc),
		"[0]*TMath::Voigt(x-[1],[2],[3])",
		10.4,13.1);
  }
  for(int ifunc = 0; ifunc != 6; ++ifunc){
    fFitResultFunction[2][ifunc]->SetParameters(fFitFunction->GetParameter(88+ifunc),
						fFitFunction->GetParameter(4*ifunc+2)+fFitFunction->GetParameter(149),
						6.21e-02,
						fFitFunction->GetParameter(4*ifunc+4));
  }
  for(int ifunc = 0; ifunc != 5; ++ifunc){
    fFitResultFunction[2][ifunc+6]->SetParameters(fFitFunction->GetParameter(94+ifunc),
						  fFitFunction->GetParameter(4*ifunc+50)+fFitFunction->GetParameter(149),
						  6.21e-02,
						  fFitFunction->GetParameter(4*ifunc+52));
  }    
  fFitResultFunction[2][11] = new TF1("f2_11",
				      "[0]+[1]*(x+4.*2.72)+[2]*pow(x+4.*2.72,2)+[3]*pow(x+4.*2.72,3)",10.4,13.1);
  fFitResultFunction[2][11]->SetParameters(fFitFunction->GetParameter(99),
					   fFitFunction->GetParameter(100),
					   fFitFunction->GetParameter(101),
					   fFitFunction->GetParameter(102));
  fFitResultFunction[2][12] = new TF1("f2_12",
				      "[0]*TMath::Voigt(x-[1],[2],[3])",
				      10.4,13.1);
  fFitResultFunction[2][12]->SetParameters(fFitFunction->GetParameter(157),
					   fFitFunction->GetParameter(154) + fFitFunction->GetParameter(149),
					   8.19648e-02,
					   fFitFunction->GetParameter(155));
  fFitResultFunction[2][13] = new TF1("f2_13",[=](double*x, double *p)
  {
    double val = 0;
    for(int ifunc = 0; ifunc != 13; ++ifunc){
      val+=fFitResultFunction[2][ifunc]->Eval(x[0]);
    }
    return val;
  },
    10.4, 13.1,0);

  // h1502 in 10025.root
  for(int ifunc = 0; ifunc != 11; ++ifunc){
    fFitResultFunction[3][ifunc]
      = new TF1(Form("f3_%d",ifunc),
		"[0]*TMath::Voigt(x-[1],[2],[3])",
		10.4,13.1);
  }
  for(int ifunc = 0; ifunc != 6; ++ifunc){
    fFitResultFunction[3][ifunc]->SetParameters(fFitFunction->GetParameter(103+ifunc),
						fFitFunction->GetParameter(4*ifunc+2)+fFitFunction->GetParameter(150),
						5.74004e-02,
						fFitFunction->GetParameter(4*ifunc+4));
  }
  for(int ifunc = 0; ifunc != 5; ++ifunc){
    fFitResultFunction[3][ifunc+6]->SetParameters(fFitFunction->GetParameter(109+ifunc),
						  fFitFunction->GetParameter(4*ifunc+50)+fFitFunction->GetParameter(150),
						  5.74004e-02,
						  fFitFunction->GetParameter(4*ifunc+52));
  }    
  fFitResultFunction[3][11] = new TF1("f3_11",
				      "[0]+[1]*(x+6.*2.72)+[2]*pow(x+6.*2.72,2)+[3]*pow(x+6.*2.72,3)",10.4,13.1);
  fFitResultFunction[3][11]->SetParameters(fFitFunction->GetParameter(114),
					   fFitFunction->GetParameter(115),
					   fFitFunction->GetParameter(116),
					   fFitFunction->GetParameter(117));
  fFitResultFunction[3][12] = new TF1("f3_12",
				      "[0]*TMath::Voigt(x-[1],[2],[3])",
				      10.4,13.1);
  fFitResultFunction[3][12]->SetParameters(fFitFunction->GetParameter(158),
					   fFitFunction->GetParameter(154) + fFitFunction->GetParameter(150),
					   8.19648e-02,
					   fFitFunction->GetParameter(155));
  fFitResultFunction[3][13] = new TF1("f3_13",[=](double*x, double *p)
  {
    double val = 0;
    for(int ifunc = 0; ifunc != 13; ++ifunc){
      val+=fFitResultFunction[3][ifunc]->Eval(x[0]);
    }
    return val;
  },
    10.4, 13.1,0);
  
  // h1503 in 10025.root
  for(int ifunc = 0; ifunc != 11; ++ifunc){
    fFitResultFunction[4][ifunc]
      = new TF1(Form("f4_%d",ifunc),
		"[0]*TMath::Voigt(x-[1],[2],[3])",
		10.4,13.1);
  }
  for(int ifunc = 0; ifunc != 6; ++ifunc){
    fFitResultFunction[4][ifunc]->SetParameters(fFitFunction->GetParameter(118+ifunc),
						fFitFunction->GetParameter(4*ifunc+2)+fFitFunction->GetParameter(151),
						5.34052e-02,
						fFitFunction->GetParameter(4*ifunc+4));
  }
  for(int ifunc = 0; ifunc != 5; ++ifunc){
    fFitResultFunction[4][ifunc+6]->SetParameters(fFitFunction->GetParameter(124+ifunc),
						  fFitFunction->GetParameter(4*ifunc+50)+fFitFunction->GetParameter(151),
						  5.34052e-02,
						  fFitFunction->GetParameter(4*ifunc+52));
  }    
  fFitResultFunction[4][11] = new TF1("f4_11",
				      "[0]+[1]*(x+8.*2.72)+[2]*pow(x+8.*2.72,2)+[3]*pow(x+8.*2.72,3)",10.4,13.1);
  fFitResultFunction[4][11]->SetParameters(fFitFunction->GetParameter(129),
					   fFitFunction->GetParameter(130),
					   fFitFunction->GetParameter(131),
					   fFitFunction->GetParameter(132));
  fFitResultFunction[4][12] = new TF1("f4_12",
				      "[0]*TMath::Voigt(x-[1],[2],[3])",
				      10.4,13.1);
  fFitResultFunction[4][12]->SetParameters(fFitFunction->GetParameter(159),
					   fFitFunction->GetParameter(154) + fFitFunction->GetParameter(151),
					   8.19648e-02,
					   fFitFunction->GetParameter(155));
  fFitResultFunction[4][13] = new TF1("f4_13",[=](double*x, double *p)
  {
    double val = 0;
    for(int ifunc = 0; ifunc != 13; ++ifunc){
      val+=fFitResultFunction[4][ifunc]->Eval(x[0]);
    }
    return val;
  },
    10.4, 13.1,0);

  // h1504 in 10025.root
  for(int ifunc = 0; ifunc != 11; ++ifunc){
    fFitResultFunction[5][ifunc]
      = new TF1(Form("f5_%d",ifunc),
		"[0]*TMath::Voigt(x-[1],[2],[3])",
		10.4,13.1);
  }
  for(int ifunc = 0; ifunc != 6; ++ifunc){
    fFitResultFunction[5][ifunc]->SetParameters(fFitFunction->GetParameter(133+ifunc),
						fFitFunction->GetParameter(4*ifunc+2)+fFitFunction->GetParameter(152),
						5.1219e-02,
						fFitFunction->GetParameter(4*ifunc+4));
  }
  for(int ifunc = 0; ifunc != 5; ++ifunc){
    fFitResultFunction[5][ifunc+6]->SetParameters(fFitFunction->GetParameter(139+ifunc),
						  fFitFunction->GetParameter(4*ifunc+50)+fFitFunction->GetParameter(152),
						  5.1219e-02,
						  fFitFunction->GetParameter(4*ifunc+52));
  }    
  fFitResultFunction[5][11] = new TF1("f5_11","[0]+[1]*(x+10.*2.72)+[2]*pow(x+10.*2.72,2)+[3]*pow(x+10.*2.72,3)",
				      10.4,13.1);
  fFitResultFunction[5][11]->SetParameters(fFitFunction->GetParameter(144),
					   fFitFunction->GetParameter(145),
					   fFitFunction->GetParameter(146),
					   fFitFunction->GetParameter(147));
  fFitResultFunction[5][12] = new TF1("f5_12",
				      "[0]*TMath::Voigt(x-[1],[2],[3])",
				      10.4,13.1);
  fFitResultFunction[5][12]->SetParameters(fFitFunction->GetParameter(160),
					   fFitFunction->GetParameter(154) + fFitFunction->GetParameter(152),
					   5.1219e-02,
					   fFitFunction->GetParameter(155));
  for(int ihist = 0; ihist != fnHist; ++ihist){
    for(int ifunc = 0; ifunc != 13; ++ifunc){
      fFitResultFunction[ihist][ifunc]->SetLineColor(kRed);
      fFitResultFunction[ihist][ifunc]->SetLineWidth(1);      
    }
  }
  fFitResultFunction[5][13] = new TF1("f5_13",[=](double*x, double *p)
  {
    double val = 0;
    for(int ifunc = 0; ifunc != 13; ++ifunc){
      val+=fFitResultFunction[5][ifunc]->Eval(x[0]);
    }
    return val;
  },
    10.4, 13.1,0);

  return ;
}
