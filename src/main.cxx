#include <iostream>
#include <unistd.h>

void usage();

int main(int argc, char **argv){
  int opt;
  int f_graph = 0;
  while((opt = getopt(argc, argv, "g")) != -1){
    switch(opt){
    case 'g':
      f_graph = 1;
      break;
    default:
      usage();
      return -1;
    }
  }
}

void usage(){
  printf("Usage: ./kinema [-g]\n");
  printf("-g: Graph will be displayed.\n");
  return;
}
