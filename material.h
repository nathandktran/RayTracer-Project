#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class Material {
  public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color& atten, Ray& scattered) const {
      return false;
    }

};

class Lambertian : public Material {
  private:
    Color albedo;

  public:
    Lambertian(const Color& albedo) : albedo(albedo) {}

    bool scatter(const Ray& r_in, const HitRecord& rec, Color& atten, Ray& scattered) const override {
      auto scatter_direction = rec.normal + random_unit_vector();

      // Catch if scatter direction is near zero
      if (scatter_direction.near_zero()) scatter_direction = rec.normal;

      scattered = Ray(rec.p, scatter_direction);
      atten = albedo;
      
      return true;
    }
};

class Metal : public Material {
  private:
    Color albedo;
  
  public:
    Metal(const Color& albedo) : albedo(albedo) {}

    bool scatter(const Ray& r_in, const HitRecord& rec, Color& atten, Ray& scattered) const override {
      Vec3 reflected = reflect(r_in.direction(), rec.normal);
      scattered = Ray(rec.p, reflected);
      atten = albedo;
      return true;
    }
};

#endif