#ifndef GENERAL_H
#define GENERAL_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>

// C++ Std Usings

using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility

inline double degrees_to_radians(double degrees) {
  return degrees * pi / 180.0;
}

inline double random_double() {
  // From [0,1)
  return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
  // Returns a random double in [min, max)
  return min + (max - min) * random_double();
}

// Common headers
#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"

#endif