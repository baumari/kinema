#include <iostream>
#include <unistd.h>
#include <KParticle.hh>
#include <cstdlib>
#include <KCollision.hh>

void Usage(){
  printf("Usage: ./kinema [-g] [-r val] p1 p2 p3 p4 Ebeam Ex\n");
  printf("   -g: Graph will be displayed.\n");
  printf("   -r: Recoil kinetic energy.\n");
  printf("Unit of energy is [MeV].\n\n");
  printf("Output: E3 E4 theta3 theta4 \n");
}

int main(int argc, char **argv){
  int opt;
  int f_graph=0;
  double RecoilEx=0;
  //  int f_recoil=0;
  while((opt = getopt(argc, argv, "hgr:")) != -1){
    switch(opt){
    case 'g':
      f_graph = 1;
      break;
    case 'r':
      //      f_recoil=1;
      RecoilEx=atof(optarg);
      break;
    case 'h':
      Usage();
      return -1;
    default:
      Usage();
      return -1;
    }
  }
  if((argc-optind)!=6){
    Usage();
    return -1;
  }

  KParticle p1(argv[optind], atof(argv[optind+4]));
  KParticle p2(argv[optind+1], 0);
  KParticle p3(argv[optind+2], RecoilEx);
  KParticle p4(argv[optind+3],
	      atof(argv[optind+4])-atof(argv[optind+5]));

  KCollision col;
  col.SetInitParticle(p1, p2);
  std::cout << &p1 << " " << col.GetAddr1() << std::endl;

  std::exit(EXIT_SUCCESS);  
}

