#ifndef WATER_SURFACE_HPP
#define WATER_SURFACE_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"
#include "render_texture.hpp"
#include "level_template.hpp"
#include "director.hpp"

#include "pointer_macros.h"
DEF_PTR(WaterSurface, WaterSurfacePtr);

class WaterSurface : public GameObject, public CollisionObject{

public:

  WaterSurface(glm::vec3 _position, int _colorMask);

  void setup();
  void update();

  void collided(CollisionObject * collidedWith);

private:

  Uniform3DGrid<int> *typeGrid;
  Uniform3DGrid<int> grid;
  Object * waterSurface;
  glm::vec3 position;
  glm::vec3 movementDirection; 
  glm::vec3 size;
  glm::vec3 startPosition;
  RenderTexture * waterDataTexture;
  RenderTexture * waterColorTexture;
  RenderTexture * waterBlockTexture;
  LevelTemplate * level;

  std::string color;
  int colorMask;

  float minX, maxX, minY, maxY, minZ, maxZ;
  glm::vec3 lowestPosition;
  float timer;

  bool checkAdjacent(glm::vec3 newPos);
  void createFluidBox(glm::vec3 newPos);
};

#endif
