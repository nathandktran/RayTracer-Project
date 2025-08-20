#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include <iostream>
#include "general.h"
#include "material.h"

using namespace std;

class Camera {
  private:
    int image_height;             // Rendered image height
    Point3 camera_center;         // Camera center
    Point3 pixel00_loc;           // Location of pixel at 0,0
    Vec3 pixel_delta_u;           // Offset to pixel to the right
    Vec3 pixel_delta_v;           // Offset to pixel below
    double pixel_samples_scale;   // Color scale factor for a sum of pixel samples
    Vec3 u, v, w;                 // Camera frame basis vectors
    Vec3 defocus_disk_u;          // Defocus disk horizontal radius
    Vec3 defocus_disk_v;          // Defocus disk vertical radius

    void initialize() {
      image_height = int(image_width / aspect_ratio);
	    image_height = image_height < 1 ? 1: image_height;

      pixel_samples_scale = 1.0 / samples_per_pixel;

      camera_center = lookfrom;

      // Viewport dimensions
      auto theta = degrees_to_radians(vfov);
      auto h = std::tan(theta / 2);
      auto viewport_height = 2 * h * focus_dist;
      auto viewport_width = viewport_height * (double(image_width) / image_height);

      // Calculate the u, v, w unit basis vectors for the camera coordinate frame
      w = unit_vector(lookfrom - lookat);
      u = unit_vector(cross(vup, w));
      v = cross(w, u);

      // Calculate vectors across the viewport edges
      // Horizontal
      auto viewport_u = viewport_width * u; // Vector across viewport horizontal edge

      // Vertical
      auto viewport_v = viewport_height * -v; // Vector down viewport vertical edge

      // Calc delta vectors between pixels
      pixel_delta_u = viewport_u / image_width;
      pixel_delta_v = viewport_v / image_height;

      // Find upper left pixel
      auto viewport_upper_left = camera_center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;

      // Calculate the camera defocus disk basis vectors
      auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
      defocus_disk_u = u * defocus_radius;
      defocus_disk_v = v * defocus_radius;
      
      pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    Color ray_color(const Ray& r, int depth, const Hittable& world) const {
      // Stop gathering light after exceeding ray bounces
      if (depth <= 0) {
        return Color(0, 0, 0);
      }

      HitRecord rec;
      if (world.hit(r, Interval(0.001, infinity), rec)) {
        Ray scattered;
        Color atten;
        if (rec.mat->scatter(r, rec, atten, scattered)) {
          return atten * ray_color(scattered, depth - 1, world);
        }
        return Color(0, 0, 0);
      }

      Vec3 unit_direction = unit_vector(r.direction());
      auto a = 0.5 * (unit_direction.y() + 1.0);
      return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0); 
    }

    Ray get_ray(int i, int j) const {
      // Construct a camera ray originiating from the defocus disk and directed at a randomly sampled
      // point around the pixel location i, j

      auto offset = sample_square();
      auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u)
                          + ((j + offset.y()) * pixel_delta_v);
      auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
      auto ray_direction = pixel_sample - ray_origin;
      auto ray_time = random_double();
      
      return Ray(ray_origin, ray_direction, ray_time);
    }

    Vec3 sample_square() const {
      // Returns the vector to a random point in the [-.5, -.5] - [+.5, +.5] unit square
      return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    Point3 defocus_disk_sample() const {
      // Returns a random point in the camera defocus disk
      auto p = random_in_unit_disk();
      return camera_center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }
  


  public:
    double aspect_ratio = 1.0;  // Ratio of image width over height
	  int image_width = 100;      // Rendered image width in pixel count
    int samples_per_pixel = 10; // Count of random samples per pixel for AA
    int max_depth = 10;         // Max number of recursive child rays

    double vfov = 90;                   // Vertical view angle (field of view)
    Point3 lookfrom = Point3(0, 0, 0);  // Point cam is looking from
    Point3 lookat = Point3(0, 0, -1);   // Point cam is looking at
    Point3 vup = Vec3(0, 1, 0);         // Camera-relative "up" direction

    double defocus_angle = 0;   // Variation angle of rays through each pixel
    double focus_dist = 10;     // Distance from camera lookfrom point to plane of perfect of focus

    void render(const Hittable& world) {
      initialize();

      cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

      for (int j = 0; j < image_height; j++) {
        std:: clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
          Color pixel_color(0, 0, 0);
          for (int sample = 0; sample < samples_per_pixel; sample++) {
            Ray r = get_ray(i, j);
            pixel_color += ray_color(r, max_depth, world);
          }
          write_color(std::cout, pixel_samples_scale * pixel_color);
        }
      }
      std::clog << "\rDone.                 \n";
    }
};

#endif