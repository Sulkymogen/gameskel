#include <game/Player.h>

namespace game {

  Player::Player(ElementType type, float x, float y, b2World *world)
    : Element(type, x, y, 0.0f, 0.0f, world)
  {
    setFunction(ElementFunction::PLAYER);
  }
  
  void Player::move(PlayerMove move){
    switch(move){
      case PlayerMove::UP:
	  setLinearVelocity(getLinearVelocity().x, -10.0f);
	break;
	
      case PlayerMove::LEFT:
	setLinearVelocity(-10.0f, getLinearVelocity().y);
	break;
	
      case PlayerMove::BOTTOM:
	setLinearVelocity(getLinearVelocity().x, +10.0f);
	break;
	
      case PlayerMove::RIGHT:
	setLinearVelocity(+10.0f, getLinearVelocity().y);
	break;
	
      case PlayerMove::STOP:
	setLinearVelocity(0.0f, 0.0f);
	
      default:
	break;
    }
    
    return;
  }
  
}