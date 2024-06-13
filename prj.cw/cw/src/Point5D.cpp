#include <Point5D.hpp>
#include <iostream>
#include <cmath>

// Constructor
Point5D::Point5D() {
  x = -1;
  y = -1;
}

// Destructor
Point5D::~Point5D() {}

// Scale the OpenCV Lab color to Lab range
void Point5D::PointLab() {
  l = l * 100 / 255;
  a = a - 128;
  b = b - 128;
}

// Sclae the Lab color to OpenCV range that can be used to transform to RGB
void Point5D::PointRGB() {
  l = l * 255 / 100;
  a = a + 128;
  b = b + 128;
}

// Accumulate points
void Point5D::MSPoint5DAccum(Point5D Pt) {
  x += Pt.x;
  y += Pt.y;
  l += Pt.l;
  a += Pt.a;
  b += Pt.b;
}

// Copy a point
void Point5D::MSPoint5DCopy(Point5D Pt) {
  x = Pt.x;
  y = Pt.y;
  l = Pt.l;
  a = Pt.a;
  b = Pt.b;
}

// Compute color space distance between two points
float Point5D::MSPoint5DColorDistance(Point5D Pt) {
  return std::sqrt((l - Pt.l) * (l - Pt.l) + (a - Pt.a) * (a - Pt.a) +
              (b - Pt.b) * (b - Pt.b));
}

// Compute spatial space distance between two points
float Point5D::MSPoint5DSpatialDistance(Point5D Pt) {
  return std::sqrt((x - Pt.x) * (x - Pt.x) + (y - Pt.y) * (y - Pt.y));
}

// Scale point
void Point5D::MSPoint5DScale(float scale) {
  x *= scale;
  y *= scale;
  l *= scale;
  a *= scale;
  b *= scale;
}

// Set point value
void Point5D::MSPOint5DSet(float px, float py, float pl, float pa, float pb) {
  x = px;
  y = py;
  l = pl;
  a = pa;
  b = pb;
}

// Print 5D point
void Point5D::Print() {
  std::cout << x << " " << y << " " << l << " " << a << " " << b << std::endl;
}