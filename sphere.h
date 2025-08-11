#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include <cmath>
#include "material.h"

class Sphere : public Hittable {
  private:
    Point3 center;
    double radius;
    shared_ptr<Material> mat;

  public:
    Sphere(const Point3& center, double radius, shared_ptr<Material> mat)
        : center(center), radius(std::fmax(0, radius)) {
      this->mat = mat;
    }

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
      Vec3 oc = center - r.origin();
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
      Vec3 outward_normal = (rec.p - center) / radius;
      rec.setFaceNormal(r, outward_normal);
      rec.normal = (rec.p - center) / radius;
      rec.mat = mat;

      return true;
    }
};

#endif