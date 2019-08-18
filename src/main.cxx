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
  KOptions opt;
  opt.Add("h", "help");
  opt.Add("r", "recoil", 0);
  opt.Add("", "out", "");
  opt.Add("g", "graph");  

  if(!opt.Check(argc, argv)){
    fprintf(stderr, "Invalid Usage.\n");
    std::exit(EXIT_FAILURE);
  }
  if(opt.Exist("g")){
    printf("Graphical mode using TGraph\n");
    printf("Now developing...\n");
    std::exit(EXIT_SUCCESS);    
  }
  if(opt.Exist("h")){
    Usage();
    std::exit(EXIT_SUCCESS);    
  }
  FILE *OutPutFile = stdout;
  if(opt.Exist("out")){
    OutPutFile = fopen(opt.Get("out").c_str(), "w");
    if(OutPutFile == NULL){
      fprintf(stderr, "Fail to open %s\n", opt.Get("out").c_str());
      std::exit(EXIT_FAILURE);
    }
  }
  
  KParticle p1(argv[opt.Lead()], atof(argv[opt.Lead()+4]));
  KParticle p2(argv[opt.Lead()+1], 0);
  KParticle p3(argv[opt.Lead()+2],
	       atof(argv[opt.Lead()+4])-atof(argv[opt.Lead()+5])-atof(opt.Get("r").c_str()));
  KParticle p4(argv[opt.Lead()+3], atof(opt.Get("r").c_str()));

  KCollision col(p1, p2, p3, p4);
  col.Scatt();

}

