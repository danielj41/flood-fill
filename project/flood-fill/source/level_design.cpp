#include "../headers/level_design.hpp"
  
int LevelDesign::layout[LEN_X][LEN_Y][LEN_Z] = {
  /*
	X
	  Y ZZZZZZZZZZZZZZZZZZZZ
	  Y ZZZZZZZZZZZZZZZZZZZZ
	  Y ZZZZZZZZZZZZZZZZZZZZ
  */	

  {
	{0,0,0,0,0,0,1,1,1,1,0,0,0,0},
	{0,0,0,0,0,1,0,0,0,1,0,0,0,0},
	{1,1,1,1,1,0,0,0,0,1,1,1,1,1}
  },
  {
	{0,0,0,0,0,0,1,1,1,1,0,0,0,0},
	{0,0,0,0,0,1,0,0,0,1,0,0,0,0},
	{1,1,1,1,1,0,0,0,0,1,1,1,1,1}
  },
  {
	{0,0,0,0,0,0,1,1,1,1,0,0,0,0},
	{0,0,0,0,0,1,0,0,0,1,0,0,0,0},
	{1,1,1,1,1,0,0,0,0,1,1,1,1,1}
  },
  {
	{0,0,0,0,0,0,1,1,1,1,0,0,0,0},
	{0,0,0,0,0,1,0,0,0,1,0,0,0,0},
	{1,1,1,1,1,0,0,0,0,1,1,1,1,1}
  }
};

  


int LevelDesign::getVal(int x, int y, int z){
  return layout[x][y][z];
}

int LevelDesign::getLenX() {
  return LEN_X;
}

int LevelDesign::getLenY() {
  return LEN_Y;
}

int LevelDesign::getLenZ() {
  return LEN_Z;
}

