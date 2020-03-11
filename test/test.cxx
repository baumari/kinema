#include <iostream>
#include <KParticle.hh>
#include <KCollision.hh>

int main(){
  KParticle p1("a", 400);
  KParticle p2("13c");  KParticle p3("a");  KParticle p4("13c");
  p4.SetEx(10.3);
  KCollision col(p1, p2, p3, p4);
  col.SetScattAngle(0.0);
  int NumOfScatt = col.Scatt();
  for(int i = 0; i != NumOfScatt; ++i){
    std::cout << col.GetT3(i) << " " << col.GetT4(i) << std::endl;
  }
  return 0;
}
