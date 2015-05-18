#ifndef LEVEL_DESIGN_H
#define LEVEL_DESIGN_H

#define LEN_X 4
#define LEN_Y 3
#define LEN_Z 14

#include "pointer_macros.h"
DEF_PTR(LevelDesign, LevelDesignPtr);

class LevelDesign {
  
public:

static int getVal(int x, int y, int z);
static int getLenX();
static int getLenY();
static int getLenZ();
  
private:

static int layout[LEN_X][LEN_Y][LEN_Z];



};
  
#endif
