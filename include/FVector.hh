#ifndef FVector_HH
#define FVector_HH
class FVector
{
private:
  double m_p[4];
public:
  FVector();
  double e();
  double px();
  double py();
  double pz();
};

double operator*(FVector, FVector);


#endif
