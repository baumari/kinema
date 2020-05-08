#include <iostream>
#include <KParticle.hh>
#include <KCollision.hh>
#include <cstdlib>

int main(int argc, char **argv){
  double center = atof(argv[1]);
  double angle = atof(argv[2]);
  KParticle p1("a", 388);
  KParticle p2("13c");  KParticle p3("a");  KParticle p4("13c");
  p1.Show(); p2.Show();
  KCollision col;

  return 0;
}
