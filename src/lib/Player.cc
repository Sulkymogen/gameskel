#include <game/Player.h>
#include <iostream>

namespace game {

  Player::Player(ElementType type, float x, float y, b2World *world)
    : Element(type, x, y, 0.0f, 0.0f, world)
  {
    setFunction(ElementFunction::PLAYER);
    setFilter(ElementFunction::PLAYER, ElementFunction::ENEMY|ElementFunction::PLAYER|ElementFunction::BOUNDARY);
  }
  
  void Player::move(float vx, float vy){
    setLinearVelocity(vx, vy);
    
    return;
  }
  
}