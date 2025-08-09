#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "interval.h"

class HitRecord {
  public:
    Point3 p;
    Vec3 normal;
    double t;
    bool front_face;

    void setFaceNormal(const Ray& r, const Vec3& outward_normal) {
      // Sets the hit record normal vector

      front_face = dot(r.direction(), outward_normal) < 0;
      normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
  public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const = 0;
};

#endif