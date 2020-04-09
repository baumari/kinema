#include <iostream>
#include <KParticle.hh>
#include <KCollision.hh>
#include <cstdlib>

int main(int argc, char **argv){
  double center = atof(argv[1]);
  double angle = atof(argv[2]);
  KParticle p1("a", 388);
  KParticle p2("13c");  KParticle p3("a");  KParticle p4("13c");

  KCollision col;
  for(double ex = center-0.5; ex <= center+0.5; ex += 0.1){
    p4.SetEx(ex);
    col.Init();
    col.SetInitParticle(p1, p2);
    col.SetFinParticle(p3, p4);
    col.SetScattAngle(angle);
    col.Scatt();
    std::cout << "ex: " << ex << " theta: " << col.GetTheta3CM()
	      << " fac: " << col.GetFac() << std::endl;
  }
  return 0;
}
