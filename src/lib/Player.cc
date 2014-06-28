#include <game/Player.h>

namespace game {

  Player::Player(ElementType type, float x, float y, b2World *world)
    : Element(type, x, y, 0.0f, 0.0f, world)
  {
    setFunction(ElementFunction::PLAYER);
    setFilter(ElementFunction::PLAYER, ElementFunction::ENEMY|ElementFunction::PLAYER|ElementFunction::BOUNDARY);
  }
  
  void Player::move(PlayerMove move){
    switch(move){
      case PlayerMove::UP:
	  setLinearVelocity(getLinearVelocity().x, -PLAYER_SPEED);
	break;
	
      case PlayerMove::LEFT:
	setLinearVelocity(-PLAYER_SPEED, getLinearVelocity().y);
	break;
	
      case PlayerMove::BOTTOM:
	setLinearVelocity(getLinearVelocity().x, +PLAYER_SPEED);
	break;
	
      case PlayerMove::RIGHT:
	setLinearVelocity(+PLAYER_SPEED, getLinearVelocity().y);
	break;
	
      default:
	break;
    }
    
    return;
  }
  
  void Player::stop(PlayerMove move){
    switch(move){
      case PlayerMove::UP:
	  setLinearVelocity(getLinearVelocity().x, 0.0f);
	break;
	
      case PlayerMove::LEFT:
	setLinearVelocity(0.0f, getLinearVelocity().y);
	break;
	
      case PlayerMove::BOTTOM:
	setLinearVelocity(getLinearVelocity().x, 0.0f);
	
      case PlayerMove::RIGHT:
	setLinearVelocity(0.0f, getLinearVelocity().y);
	break;
	
      default:
	break;
    }
    
    return;
  }
  
}