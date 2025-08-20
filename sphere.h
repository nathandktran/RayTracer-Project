#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include <cmath>
#include "material.h"
#include "ray.h"

class Sphere : public Hittable {
  private:
    Ray center;
    double radius;
    shared_ptr<Material> mat;

  public:
    // Stationary
    Sphere(const Point3& static_center, double radius, shared_ptr<Material> mat)
        : center(static_center, Vec3(0, 0, 0)), radius(std::fmax(0, radius)), mat(mat) {}

    // Moving
    Sphere(const Point3& center1, const Point3& center2,  double radius, shared_ptr<Material> mat)
        : center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(mat) {}

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
      Point3 curr_center = center.at(r.time());
      Vec3 oc = curr_center - r.origin();
      auto a = r.direction().length_squared();
      auto h = dot(r.direction(), oc);
      auto c = oc.length_squared() - radius * radius;

      auto discriminant = h * h - a * c;
      if (discriminant < 0) return false;
      auto sqrtd = std::sqrt(discriminant);

      // Find the nearest root that lies within the range
      auto root = (h - sqrtd) / a;
      if (!ray_t.surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root)) return false;
      }

      rec.t = root;
      rec.p = r.at(rec.t);
      Vec3 outward_normal = (rec.p - curr_center) / radius;
      rec.setFaceNormal(r, outward_normal);
      rec.mat = mat;

      return true;
    }
};

#endif