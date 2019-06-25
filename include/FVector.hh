#ifndef _FVECTOR_HH
#define _FVECTOR_HH

class FVector
{
private:
  double m_p[5]; // The fifth component is mass.
public:
  FVector() : m_p() {};
  ~FVector();
  double e();
  double px();
  double py();
  double pz();
  double mass();    
  void setkine(double);
  void setmass(double);
  void inite(double mass, double kine);
  void update();
  void Init();
};

double operator*(FVector, FVector);


#endif
