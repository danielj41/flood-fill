#ifndef LIGHT_H
#define LIGHT_H

#include "glm/glm.hpp"

using namespace glm;

class Light {

 public:
  Light();
  Light(vec3 pos, float intensity, vec3 color, vec3 direction); 

  //void updatePosition(vec3 pos);
  void draw(mat4 transformMat);
  
  
 private: 
  vec3 pos, color, direction;
  float intensity;

}

#endif
 
