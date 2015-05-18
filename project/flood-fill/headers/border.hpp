#ifndef BORDER_HPP
#define BORDER_HPP

#define E_LEN 2.0
#define NUMBLKX 6
#define NUMBLKY 6
#define NUMBLKZ 16
#define MINX 0.0f
#define MINY 0.0f
#define MINZ -32.0f
#define MAXX 12.0f
#define MAXZ 0.0f

#include "pointer_macros.h"
DEF_PTR(Border, BorderPtr);

class Border {
  
public:
  
  static void createBorder();
  
};

#endif
