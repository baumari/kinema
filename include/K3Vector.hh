#ifndef _K3VECTOR_HH
#define _K3VECTOR_HH

class K3Vector
{
private:
  enum K3VECTOR{_X, _Y, _Z, SIZE};  

private:
  double m_r[SIZE];

public:
  K3Vector() {}
  K3Vector(double x, double y, double z);
  ~K3Vector() {}

public:
  inline double X() const {return m_r[_X];}
  inline double Y() const {return m_r[_Y];}
  inline double Z() const {return m_r[_Z];}
  inline void SetX(double x) {m_r[_X] = x;}
  inline void SetY(double y) {m_r[_Y] = y;}
  inline void SetZ(double z) {m_r[_Z] = z;}    
  void Set(double x, double y, double z);
  double Norm() const;
  K3Vector& operator=(const K3Vector& r);
  K3Vector operator+(const K3Vector& r) const;
  K3Vector operator-(const K3Vector& r) const;
  double operator*(const K3Vector& r) const;
  inline K3Vector operator+() const {return *this;}
  K3Vector operator-() const;
  void RotateX(double theta); // rad.
  void RotateY(double theta); // rad.
  void RotateZ(double theta); // rad.
  void Show() const;
};  

#endif
