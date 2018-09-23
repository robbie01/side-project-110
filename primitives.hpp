#pragma once

struct Matrix2;

struct Vector2 {
  float x;
  float y;
};

float operator*(const Vector2&, const Vector2&);
Matrix2 operator%(const Vector2&, const Vector2&);

struct Matrix2 {
  float a;
  float b;
  float c;
  float d;

  Matrix2 operator~() const;

  static Matrix2 identity();
  static Matrix2 stretch(float, float);
  static Matrix2 rotate(float);
  static Matrix2 shear(float, float);
  static Matrix2 householder(float);
};

Matrix2 operator*(const Matrix2&, const Matrix2&);
Vector2 operator*(const Matrix2&, const Vector2&);

Matrix2 operator+(const Matrix2&, const Matrix2&);
Matrix2 operator-(const Matrix2&, const Matrix2&);

Matrix2 operator*(const Matrix2&, float);
Matrix2 operator/(const Matrix2&, float);
Matrix2 operator+(const Matrix2&, float);
Matrix2 operator-(const Matrix2&, float);


struct Vector2I {
  int x, y;
};

struct Camera {
  float near, cx, cy, cz, rx, ry, rz;
  Vector2I project(float, float, float) const;
};

struct Rect {
  int x1, y1, x2, y2;
};

struct Triangle {
  Vector2I v1, v2, v3;
  Rect getBoundingRect() const;
  bool hasPoint(const Vector2I&) const;
};

struct Rect3D {
  int x1, y1, z1, x2, y2, z2;
};

struct QuadGeometry {
  Triangle t1, t2;
  int color;

  static QuadGeometry projectRect(const Camera&, const Rect3D&, int);
};
