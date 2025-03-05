#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"
#include "material.h"

class triangle : public hittable {
  public:
    triangle(const point3& v0, const point3& v1, const point3& v2, shared_ptr<material> mat)
      : v0(v0), v1(v1), v2(v2), mat(mat) {
        edge0 = v1-v0;
        edge1 = v2-v1;
        edge2 = v0-v2;
        planeNormal = cross(edge0, edge1);
        planeNormal = planeNormal/planeNormal.length();
        std::cout << planeNormal;
      }
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
      //p = intersection point of a ray with a plane
      double d = -dot(planeNormal, v0);
      auto denom = dot(planeNormal, r.direction());
      //if ray is parallel
      if (denom == 0) 
        return false;
      double t = -(dot(planeNormal,r.origin()) + d)/denom; 
      point3 P = r.at(t);

      vec3 c0 = P - v0;
      vec3 c1 = P - v1;
      vec3 c2 = P - v2;
      if (dot(planeNormal, cross(edge0, c0)) > 0 &&
          dot(planeNormal, cross(edge1, c1)) > 0 &&
          dot(planeNormal, cross(edge2, c2)) > 0) {
            rec.t = t;
            rec.p = P;
            rec.set_face_normal(r,planeNormal);
            rec.mat = mat;
            return true;
          }
      else return false;

    }



  private:
    point3 v0;
    point3 v1;
    point3 v2;
    shared_ptr<material> mat;
    vec3 edge0;
    vec3 edge1;
    vec3 edge2;
    vec3 planeNormal;

};

#endif