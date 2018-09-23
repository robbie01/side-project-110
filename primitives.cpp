#include <algorithm>
#include <cmath>

#include "primitives.hpp"

using std::min, std::max;

float operator*(const Vector2& a, const Vector2& b) {
  return (a.x*b.x) + (a.y*b.y);
}

Matrix2 operator%(const Vector2& a, const Vector2& b) {
  return Matrix2{a.x*b.x, a.x*b.y, a.y*b.x, a.y*b.y};
}

Matrix2 Matrix2::operator~() const {
  const Matrix2& a = *this;
  return Matrix2{a.d, -a.c, -a.b, a.a} / ((a.a*a.d)-(a.b*a.c));
}

Matrix2 operator*(const Matrix2& a, const Matrix2& b) {
  return Matrix2{
    Vector2{a.a, a.b} * Vector2{b.a, b.c},
    Vector2{a.a, a.b} * Vector2{b.b, b.d},
    Vector2{a.c, a.d} * Vector2{b.a, b.c},
    Vector2{a.c, a.d} * Vector2{b.b, b.d},
  };
}

Vector2 operator*(const Matrix2& a, const Vector2& b) {
  return Vector2{
    Vector2{a.a, a.b} * b,
    Vector2{a.c, a.d} * b
  }; 
}

Matrix2 operator+(const Matrix2& a, const Matrix2& b) {
  return Matrix2{
    a.a+b.a,
    a.b+b.b,
    a.c+b.c,
    a.d+b.d
  };
}

Matrix2 operator-(const Matrix2& a, const Matrix2& b) {
  return Matrix2{
    a.a-b.a,
    a.b-b.b,
    a.c-b.c,
    a.d-b.d
  };
}

Matrix2 operator+(const Matrix2& a, float b) {
  return Matrix2{
    a.a+b,
    a.b+b,
    a.c+b,
    a.d+b
  };
}

Matrix2 operator-(const Matrix2& a, float b) {
  return Matrix2{
    a.a-b,
    a.b-b,
    a.c-b,
    a.d-b
  };
}

Matrix2 operator*(const Matrix2& a, float b) {
  return Matrix2{
    a.a*b,
    a.b*b,
    a.c*b,
    a.d*b
  };
}

Matrix2 operator/(const Matrix2& a, float b) {
  return Matrix2{
    a.a/b,
    a.b/b,
    a.c/b,
    a.d/b
  };
}

Matrix2 Matrix2::identity() {
  return Matrix2{1, 0, 0, 1};
}

Matrix2 Matrix2::stretch(float x, float y) {
  return Matrix2{x, 0, 0, y};
}

Matrix2 Matrix2::rotate(float r) {
  float si = std::sin(r), co = std::cos(r);
  return Matrix2{co, si, -si, co};
}

Matrix2 Matrix2::shear(float x, float y) {
  return Matrix2{x*y+1, x, y, 1};
}

Matrix2 Matrix2::householder(float r) {
  Vector2 vec{std::cos(r), std::sin(r)};
  return Matrix2::identity()-vec%vec*2;
}

Rect Triangle::getBoundingRect() const {
  int minx = min(v1.x, min(v2.x, v3.x));
  int miny = min(v1.y, min(v2.y, v3.y));
  int maxx = max(v1.x, max(v2.x, v3.x));
  int maxy = max(v1.y, max(v2.y, v3.y));
  return Rect{minx, miny, maxx, maxy};
}

bool edge(const Vector2I& a, const Vector2I& b, const Vector2I& c) {
  return (c.x-a.x)*(b.y-a.y)-(c.y-a.y)*(b.x-a.x) >= 0;
}

bool Triangle::hasPoint(const Vector2I& p) const {
  return edge(v1, v2, p) &&
         edge(v2, v3, p) &&
         edge(v3, v1, p);
}

Vector2I Camera::project(float ax, float ay, float az) const {
  float
    x = ax-cx,
    y = ay-cy,
    z = az-cz,
    sx = sin(rx),
    sy = sin(ry),
    sz = sin(rz), 
    cx = cos(rx),
    cy = cos(ry),
    cz = cos(rz),
    dx = cy * (sz*y + cz*x) - sy*z,
    dy = sx * (cy*z + sy*(sz*y + cz*x)) + cx * (cz*y - sz*x),
    dz = cx * (cy*z + sy*(sz*y + cz*x)) - sx * (cz*y - sz*x),
    bx = dx * near / dz + w/2,
    by = dy * near / dz + h/2;
  return Vector2I{ (int)bx, (int)by };
}

QuadGeometry QuadGeometry::projectRect(const Camera& cam, const Rect3D& rect, int color) {
  Vector2I
    v1 = cam.project(rect.x1, rect.y1, rect.z1),
    v2 = cam.project(rect.x2, rect.y1, rect.z2),
    v3 = cam.project(rect.x1, rect.y2, rect.z1),
    v4 = cam.project(rect.x2, rect.y2, rect.z2);
  return QuadGeometry{ Triangle{v1, v2, v3}, Triangle{v4, v3, v2}, color };
}
