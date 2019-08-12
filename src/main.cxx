#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include <KParticle.hh>
#include <KCollision.hh>
#include <KOptions.hh>

void Usage(){
  printf("Usage: ./kinema p1 p2 p3 p4 Ebeam Ex [options]\n");
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
  Opt.Add("out","","out.dat");

  if(!Opt.Check(argc, argv)){
    Usage();
    std::exit(EXIT_FAILURE);
  }
  if(Opt.Exist("h")){
    Usage();
    std::exit(EXIT_SUCCESS);
  }
//  if(Opt.Exist("recoil")){
//    RecEne=Opt.Get("recoil");
//  }
//  if(Opt.Exist("out")){
//    os.Open((Opt.Get("out")));
//  }else os=std::cout;
//
//  if(argc < 6){
//    Usage();
//    std::exit(EXIT_FAILURE);
//  }
  
  //  int f_recoil=0;

//  KParticle p1(argv[optind], atof(argv[optind+4]));
//  KParticle p2(argv[optind+1], 0);
//  KParticle p3(argv[optind+2], RecoilEx);
//  KParticle p4(argv[optind+3],
//	       atof(argv[optind+4])-atof(argv[optind+5]));
  
//  KParticle p1;
//  KParticle p2;
//  KParticle p3;  
//  KParticle p4;
//
//  KCollision col(p1, p2, p3, p4);  
//  col.Scatt();  
  std::exit(EXIT_SUCCESS);  
}

