#include <iostream>
#include <unistd.h>
#include <particle.hh>
#include <myerrno.hh>

void usage();

_ERR ERR = {}; // extern defined

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
      usage();
      return -1;
    default:
      usage();
      return -1;
    }
  }
  if((argc-optind)!=6){
    usage();
    return -1;
  }
  Particle p1(argv[optind], atof(argv[optind+4]));
  Particle p2(argv[optind+1], 0);
  Particle p3(argv[optind+2], RecoilEx);
  Particle p4(argv[optind+3],
	      atof(argv[optind+4])-atof(argv[optind+5]));
  CheckERR();

  return 0;  
}

void usage(){
  printf("Usage: ./kinema [-g] [-r val] p1 p2 p3 p4 Ebeam Ex\n");
  printf("   -g: Graph will be displayed.\n");
  printf("   -r: Recoil kinetic energy.\n");
  printf("Unit of energy is [MeV].\n\n");
  printf("Output: E3 E4 theta3 theta4 \n");
  return;
}
