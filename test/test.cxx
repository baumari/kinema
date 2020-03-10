#include <iostream>
#include <KParticle.hh>
#include <KCollision.hh>

int main(){
  KParticle p1("a", 388);
  KParticle p2("p");  KParticle p3("a");  KParticle p4("p");  
  KCollision col;
  col.SetInitParticle(p1, p2);
  col.SetScattAngle(2.5);
  col.Scatt();
  for(int i = 0; i != col.GetParticleNum(); ++i){
    std::cout << col.GetT4(i) << std::endl;
  }


  return 0;
}
