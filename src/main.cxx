#include <iostream>
#include <unistd.h>
#include <Particle.hh>
#include <cstdlib>
#include <Util.hh>

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
  Particle p1(argv[optind], atof(argv[optind+4]));
  Particle p2(argv[optind+1], 0);
  Particle p3(argv[optind+2], RecoilEx);
  Particle p4(argv[optind+3],
	      atof(argv[optind+4])-atof(argv[optind+5]));

  if(p1.Fail() || p2.Fail() || p3.Fail() || p4.Fail()){
    std::exit(EXIT_FAILURE);
  }

  std::exit(EXIT_SUCCESS);  
}

