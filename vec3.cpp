#include "vec3.h"

#include <cassert>
#include <iostream>
#include <math.h>
#include <iomanip>

#define SMALL_VALUE 1e-6
#define EQ(a, b) abs(a-b) < SMALL_VALUE

// Default constructor:  initializes a 0x0 vector.
Vec3::Vec3() 
{
    x = y = z = 0;
}

// Copy constructor:
Vec3::Vec3(const Vec3 &v) 
{
    copy(v);  // simply call helper function to copy
}

Vec3::Vec3(double aX, double aY, double aZ)
    : x(aX)
    , y(aY)
    , z(aZ)
{
}

// Private helper functions for constructors/destructors/assignemnt operator

// copy contents of v into the object
void Vec3::copy(const Vec3 &v) {

  x = v.x;
  y = v.y;
  z = v.z;
}

// Assignment operator that checks for self-assignment
Vec3 & Vec3::operator=(const Vec3 &rhs) 
{
    // Only do assignment if RHS is a different object from this.
    if (this != &rhs) {
        copy(rhs);
    }
    return *this;
}

// add a vector to self
Vec3 & Vec3::operator+=(const Vec3 &rhs) 
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

// subtract a vector from self
Vec3 & Vec3::operator-=(const Vec3 &rhs) 
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;

    return *this;
}

// return true iff each element of self is equal to argument vector
bool Vec3::operator==(const Vec3 &other) const 
{
    // Compare the values, and return a bool result.
    if (!EQ(x, other.x) || !EQ(y, other.y) || !EQ(z, other.z)) {
        return false;
    }
    return true;
} 

// return true iff at least 1 element of self does not match argument vector
bool Vec3::operator!=(const Vec3 &other) const {
  return !(*this == other);  // must be opposite of == operator.
}  

void Vec3::add(double v)
{
    x += v;
    y += v;
    z += v;
}

void Vec3::subtract(double v)
{
    x -= v;
    y -= v;
    z -= v;
}

void Vec3::multiply(double v)
{
    x *= v;
    y *= v;
    z *= v;
}

void Vec3::divide(double v)
{
    assert(v != 0);
    x /= v;
    y /= v;
    z /= v;
}

// return length of vector
double Vec3::length() const 
{
    return sqrt(x * x + y * y + z * z);
}

// returns the dot product of two equal sized vectors
// (each vector must have only 1 column, and have the same number of rows)
double Vec3::dot(const Vec3 &other) const 
{
    return x * other.x + y * other.y + z * other.z;
}

// normalize a vector.
void Vec3::normalize() 
{
    double len = length();
    if (len != 0) {
        x = x / len;
        y = y / len;
        z = z / len;
    } else {
        std::cerr << "cannot normalize a vector with length 0" << std::endl;
    }
}

// return vector that is a cross product
Vec3 Vec3::cross(const Vec3 &v) const 
{
    double X = (y * v.z) - (z * v.y);
    double Y = (x * v.z) - (z * v.x);
    double Z = (x * v.y) - (y * v.x);

    Vec3 result(X, Y, Z);
    return result;
}

// add operation to create a new vector object
const Vec3 Vec3::operator+(const Vec3 &v) const {

  Vec3 result(*this);   // make a copy of self
  result += v; // add the RHS to the result.
  return result;
}

// subtract operation to create a new vector object
const Vec3 Vec3::operator-(const Vec3 &v) const {

  Vec3 result(*this);  // make a copy of self
  result -= v;  // subtract the RHS from the result
  return result;
}  

void Vec3::zeroClip()
{
    if (x < 0.0) {
        x = 0.0;
    }
    if (y < 0.0) {
        y = 0.0;
    }
    if (z < 0.0) {
        z = 0.0;
    }
}

void Vec3::oneClip()
{
    if (x > 1.0) {
        x = 1.0;
    }
    if (y > 1.0) {
        y = 1.0;
    }
    if (z > 1.0) {
        z = 1.0;
    }
}

