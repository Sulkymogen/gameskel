#include <game/Bonus.h>
#include <iostream>

namespace game {

  Bonus::Bonus(float x, float y, float vx, float vy, b2World *world)
    : Element(ElementType::BONUS, x, y, vx, vy, world){
  }
}
  
  
 
