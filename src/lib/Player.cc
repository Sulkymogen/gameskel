#include <game/Player.h>

namespace game {

  Player::Player(ElementType type, float x, float y, b2World *world)
    : Element(type, x, y, 0.0f, 0.0f, world)
  {
    setFunction(ElementFunction::PLAYER);
  }
  
}