#include <game/Player.h>

namespace game {

  Player::Player(ElementType type, b2World *world)
    : Element(type, world)
  {
    setFunction(ElementFunction::PLAYER);
  }
  
}