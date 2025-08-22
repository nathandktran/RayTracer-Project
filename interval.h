#ifndef INTERVAL_H
#define INTERVAL_H

#include "general.h"

class Interval {
  public:
    double min, max;
    
    Interval() : min(+infinity), max(-infinity) {} // Default interval is empty

    Interval(double min, double max) : min(min), max(max) {}

    Interval(const Interval& a, const Interval& b) {
      // Create the interval tightly enclosing the two input intervals

      min = a.min <= b.min ? a.min : b.min;
      max = a.max >= b.max? a.max : b.max;
    }

    double size() const {
      return max - min;
    }

    bool contains(double x) const {
      return min <= x && x <= max;
    }

    bool surrounds(double x) const {
      return min < x && x < max;
    }

    double clamp(double x) const {
      if (x < min) return min;
      if (x > max) return max;
      return x;
    }

    Interval expand(double delta) const {
      auto padding = delta / 2;
      return Interval(min - padding, max + padding);
    }

    static const Interval empty, universe;
    
};

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);

#endif