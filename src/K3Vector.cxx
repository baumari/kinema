#include <K3Vector.hh>
#include <cmath>
#include <iostream>
#include <cstdio>

K3Vector::K3Vector(double x, double y, double z)
{
  m_r[_X] = x; m_r[_Y] = y; m_r[_Z] = z;
}

void K3Vector::Set(double x, double y, double z)
{
  m_r[_X] = x; m_r[_Y] = y; m_r[_Z] = z;
}

K3Vector& K3Vector::operator=(const K3Vector& r)
{
  m_r[_X] = r.X();
  m_r[_Y] = r.Y();
  m_r[_Z] = r.Z();
  return *this;
}

K3Vector K3Vector::operator+(const K3Vector& r) const
{
  K3Vector tmp;
  tmp.SetX(m_r[_X] + r.X());
  tmp.SetY(m_r[_Y] + r.Y());
  tmp.SetZ(m_r[_Z] + r.Z());
  return tmp;
}

K3Vector K3Vector::operator-(const K3Vector& r) const
{
  K3Vector tmp;
  tmp.SetX(m_r[_X] - r.X());
  tmp.SetY(m_r[_Y] - r.Y());
  tmp.SetZ(m_r[_Z] - r.Z());
  return tmp;
}

double K3Vector::operator*(const K3Vector& r) const
{
  return m_r[_X]*r.X()+m_r[_Y]*r.Y()+m_r[_Z]*r.Z();
}

K3Vector K3Vector::operator-() const
{
  K3Vector tmp;
  tmp.SetX(-m_r[_X]); tmp.SetY(-m_r[_Y]); tmp.SetZ(-m_r[_Z]);
  return tmp;
}

double K3Vector::Norm() const
{
  return sqrt(pow(m_r[_X],2)+pow(m_r[_Y],2)+pow(m_r[_Z],2));
}

void K3Vector::RotateX(double theta)
{
  Set(m_r[_X], cos(theta)*m_r[_Y]-sin(theta)*m_r[_Z],
      sin(theta)*m_r[_Y]+cos(theta)*m_r[_Z]);
}

void K3Vector::RotateY(double theta)
{
  Set(sin(theta)*m_r[_Z] + cos(theta)*m_r[_X],
      m_r[_Y], cos(theta)*m_r[_Z] - sin(theta)*m_r[_X]);
}

void K3Vector::RotateZ(double theta)
{
  Set(cos(theta)*m_r[_X] - sin(theta)*m_r[_Y],
      sin(theta)*m_r[_X] + cos(theta)*m_r[_Y], m_r[_Z]);
}


void K3Vector::Show() const
{
  printf("(%lf, %lf, %lf)\n",m_r[_X],m_r[_Y],m_r[_Z]);
}
