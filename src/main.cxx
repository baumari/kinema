#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include <KParticle.hh>
#include <KCollision.hh>
#include <KOptions.hh>

void Usage(){
  printf("Usage: ./kinema ([options]) p1 p2 p3 p4 Ebeam Ex [options]\n");
  printf("Unit of energy is [MeV].\n");
  printf("Output: E3 E4 theta3 theta4 \n\n");  
  printf("Options..\n");
  printf("-r, --recoil : Recoil kinetic energy (double [=0])\n");
  printf("    --out    : Output file (string [=out.dat])\n");
  printf("-h, --help   : Show help.\n");
}

int main(int argc, char* argv[]){

  KOptions Opt;
  Opt.Add("help","h");
  Opt.Add("recoil","r",0);
  Opt.Add("out","","");

  if(!Opt.Check(argc, argv)){
    std::exit(EXIT_FAILURE);
  }
  if(Opt.Exist("h")){
    Usage();
    std::exit(EXIT_SUCCESS);
  }
  if(argc < 6){
    Usage();
    std::exit(EXIT_FAILURE);
  }

  FILE *fp;  
  if(Opt.Exist("out")){
    fp = fopen(Opt.Get("out").c_str(), "w");
    if(fp == NULL){
      fprintf(stderr, "File %s cannot be opened..\n", Opt.Get("out").c_str());
      std::exit(EXIT_FAILURE);
    }
  }else fp = stdout;

  KParticle p1(argv[Opt.LeadArg], atof(argv[Opt.LeadArg+4]));  
  KParticle p2(argv[Opt.LeadArg+1], 0);
  KParticle p3(argv[Opt.LeadArg+2], atof(Opt.Get("recoil").c_str()));
  KParticle p4(argv[Opt.LeadArg+3], atof(argv[Opt.LeadArg+4])-atof(argv[Opt.LeadArg+5]));
  
  KCollision col(p1, p2, p3, p4);  
//  col.Scatt();

  fclose(fp);
  std::exit(EXIT_SUCCESS);  
}

