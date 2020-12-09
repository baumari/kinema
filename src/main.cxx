#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <cstring>
#include <algorithm>

#include <TCanvas.h>
#include <TGraph.h>
#include <TApplication.h>
#include <TH1F.h>

#include <KParticle.hh>
#include <KCollision.hh>
#include <KOptions.hh>
#include <K3Vector.hh>
#include <KUtil.hh>
#include <K4Momentum.hh>

void Usage(){
  printf("Usage: ./kinema ([options]) p1 p2 p3 p4 Ebeam Ex [options]\n");
  printf("Unit of energy is [MeV].\n");
  printf("Output: E3 E4 theta3 theta4 \n\n");  
}

// graphical option 
const char gopt[] = {"e3e4theta3theta4theta3cm"};

int main(int argc, char* argv[]){
  
  KOptions opt;
  opt.Add("h", "help", "Show help.");
  opt.Add("r", "recoil", 0., "Recoil kinetic energy");
  opt.Add("o", "out", "out.dat", "Output file");
  opt.Add("g","graph","theta3:e3", "Show graph. Choose graph mode.");
  opt.Constraint("graph","theta3, theta4, e3 ,e4 ,theta3cm");
  
  if(!opt.Check(argc, argv)){
    fprintf(stderr, "Invalid Usage.\n");
    std::exit(EXIT_FAILURE);
  }
  if(opt.Exist("h")){
    Usage();
    opt.Description();
    std::exit(EXIT_SUCCESS);    
  }

  FILE *OutPutFile = stdout;
  if(opt.Exist("out")){
    OutPutFile = fopen(opt.Get<std::string>("out").c_str(), "w");
    if(OutPutFile == NULL){
      fprintf(stderr, "Fail to open %s\n", opt.Get<std::string>("out").c_str());
      std::exit(EXIT_FAILURE);
    }
  }
  
  if(argc < 7){
    fprintf(stderr, "More operands are needed!!\n");
    Usage();
    std::exit(EXIT_FAILURE);    
  }
  double Ebeam = atof(argv[opt.LeadArg() + 4]);
  double Ex = atof(argv[opt.LeadArg() + 5]);
  double Erec = opt.Get<double>("recoil");
  KParticle p1(argv[opt.LeadArg()], Ebeam);
  KParticle p2(argv[opt.LeadArg() + 1]);
  KParticle p3(argv[opt.LeadArg() + 2], Erec);
  KParticle p4(argv[opt.LeadArg() + 3]);
  p4.SetEx(Ex);

  KCollision col(p1, p2, p3, p4);

  // graphical mode
  if(opt.Exist("graph")){
    TApplication app("app", &argc, argv);
    std::string sGraphMode[2];
    std::stringstream ssGraphMode(opt.Get<std::string>("graph"));
    int icnt = 0;
    while(std::getline(ssGraphMode, sGraphMode[icnt], ':')) icnt++;
    if(icnt != 2){
      std::cerr << "Invalid argument for g option!!" << std::endl;
      opt.Description();
      std::exit(EXIT_FAILURE);
    }else if(strstr(gopt, sGraphMode[0].c_str()) == NULL || strstr(gopt, sGraphMode[1].c_str()) == NULL){
      std::cerr << "No such option in g!!" << std::endl;
      opt.Description();
      std::exit(EXIT_FAILURE);      
    }
    col.SetScattAngle(0., 180., 0.1); // in CM frame
    col.Scatt();
    std::vector<double> vx, vy;
    if(sGraphMode[0] == "e3"){
      vx = col.GetT3Vec(0);
    }else if(sGraphMode[0] == "e4"){
      vx = col.GetT4Vec(0);
    }else if(sGraphMode[0] == "theta3"){
      vx = col.GetTheta3Vec();
    }else if(sGraphMode[0] == "theta4"){
      vx = col.GetTheta4Vec(0);
    }else if(sGraphMode[0] == "theta3cm"){
      vx = col.GetTheta3CMVec(0);
    }else{
      std::cerr << "Unrecognized variable: " << sGraphMode[0]
		<< std::endl;
      std::exit(EXIT_FAILURE);
    }
    if(sGraphMode[1] == "e3"){
      vy = col.GetT3Vec(0);
    }else if(sGraphMode[1] == "e4"){
      vy = col.GetT4Vec(0);
    }else if(sGraphMode[1] == "theta3"){
      vy = col.GetTheta3Vec();
    }else if(sGraphMode[1] == "theta4"){
      vy = col.GetTheta4Vec(0);
    }else if(sGraphMode[1] == "theta3cm"){
      vy = col.GetTheta3CMVec(0);
    }else{
      std::cerr << "Unrecognized variable: " << sGraphMode[1]
		<< std::endl;
      std::exit(EXIT_FAILURE);
    }
    TCanvas *c = new TCanvas();
    TVirtualPad *pad = c->cd();
    double xmin, xmax, ymin, ymax;
    xmin = *std::min_element(vx.begin(), vx.end())-1. < 0 ? 0 : *std::min_element(vx.begin(), vx.end())-1.;
    xmax = *std::max_element(vx.begin(), vx.end())+1.;
    ymin = *std::min_element(vy.begin(), vy.end())-1. < 0 ? 0 : *std::min_element(vy.begin(), vy.end())-1.;
    ymax = *std::max_element(vy.begin(), vy.end())+1.;
    TH1F *frame = pad->DrawFrame(xmin, ymin, xmax, ymax);
    frame->GetXaxis()->SetTitle(sGraphMode[0].c_str());
    frame->GetYaxis()->SetTitle(sGraphMode[1].c_str()); 
    frame->SetTitle(Form("%s + %s -> %s + %s @ %.1lf MeV, Ex = %.1lf MeV",p1.GetName().c_str(), p2.GetName().c_str(), p3.GetName().c_str(),
		     p4.GetName().c_str(), Ebeam, Ex ));   
    TGraph *g = new TGraph(vx.size(), &vx[0], &vy[0]);
    g->SetMarkerStyle(8);
    g->SetMarkerSize(1);
    g->Draw("l");
    c->Update();
    std::cout << "The maximum " << sGraphMode[0] << " is " << *std::max_element(vx.begin(), vx.end())
	      << ", the minimum is " << *std::min_element(vx.begin(), vx.end()) << std::endl;
    std::cout << "The maximum " << sGraphMode[1] << " is " << *std::max_element(vy.begin(), vy.end())
	      << ", the minimum is " << *std::min_element(vy.begin(), vy.end()) << std::endl;    
    app.Run();
  }
  col.SetScattAngle(0., 180., 0.1); // in CM frame
  col.Scatt();    
  col.ResultDump(OutPutFile);

  fclose(OutPutFile);

  std::exit(EXIT_SUCCESS);
}

