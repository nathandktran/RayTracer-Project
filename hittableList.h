#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include "hittable.h"

#include <vector>
#include <iostream>

using std::make_shared;
using std::shared_ptr;

class HittableList : public Hittable {
  public:
    std::vector<shared_ptr<Hittable>> objects;

    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<Hittable> object) {
      objects.push_back(object);
    }

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
      HitRecord temp_rec;
      bool hit_anything = false;
      auto curr_closest = ray_t.max;

      for (const auto& object : objects) {
        if (object->hit(r, Interval(ray_t.min, curr_closest), temp_rec)) {
          hit_anything = true;
          curr_closest = temp_rec.t;
          rec = temp_rec;
        }
      }

      return hit_anything;
    }
};

#endif