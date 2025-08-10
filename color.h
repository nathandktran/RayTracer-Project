#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "general.h"
#include "interval.h"

using Color = Vec3;

void write_color(std::ostream& out, const Color& pixel_color) {
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  // Convert the [0,1] range to byte range [0, 255]
  static const Interval intensity(0.000, 0.999);
  int rbyte = int(255.999 * intensity.clamp(r));
  int gbyte = int(255.999 * intensity.clamp(g));
  int bbyte = int(255.999 * intensity.clamp(b));

  // Write out the pixel color components
  out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif