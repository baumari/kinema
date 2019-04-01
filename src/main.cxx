#include <iostream>
#include <unistd.h>
#include <particle.hh>

void usage();

int main(int argc, char **argv){
  int opt;
  int f_graph=0;
  double rece=0;
  //  int f_recoil=0;
  while((opt = getopt(argc, argv, "gr:")) != -1){
    switch(opt){
    case 'g':
      f_graph = 1;
      break;
    case 'r':
      //      f_recoil=1;
      rece=atof(optarg);
      break;
    default:
      usage();
      return -1;
    }
  }
  if((argc-optind)!=6){
    usage();
    return -1;
  }
  particle p1(argv[optind], atof(argv[optind+4]));
  particle p2(argv[optind+1], 0);
  particle p3(argv[optind+2], rece);
  particle p4(argv[optind+3], atof(argv[optind+4])-atof(argv[optind+5]));
  return 0;  
}

void usage(){
  printf("Usage: ./kinema [-g] [-r val] p1 p2 p3 p4 Ebeam Ex\n");
  printf("   -g: Graph will be displayed.\n");
  printf("   -r: Recoil kinetic energy.\n");
  printf("Unit of energy is [MeV].\n");
  return;
}
