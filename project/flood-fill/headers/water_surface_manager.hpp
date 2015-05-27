#ifndef WATER_SURFACE_MANAGER_HPP
#define WATER_SURFACE_MANAGER_HPP

#include "glm/glm.hpp"

#include "game_object.hpp"
#include "water_surface.hpp"
#include "director.hpp"

#include "pointer_macros.h"
DEF_PTR(WaterSurfaceManager, WaterSurfaceManagerPtr);

class WaterSurfaceManager : public GameObject {

public:
  WaterSurfaceManager();

  void setup();
  void update();

  void addWaterSurface(glm::vec3 newPos, int colorMask);

private:
  WaterSurfacePtr current;
  glm::vec3 nextPos;
  int nextColorMask;
  float hasNextTimer;
};

#endif
