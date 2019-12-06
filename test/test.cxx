#include <iostream>
#include <KParticle.hh>

int main(){
  KParticle *p = new KParticle("a",400);
  p->P().Show();
  delete p;
  return 0;
}
