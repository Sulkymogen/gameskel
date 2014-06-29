#include <game/Player.h>
#include <iostream>

namespace game {

  Player::Player(ElementType type, float x, float y, b2World *world)
    : Element(type, x, y, 0.0f, 0.0f, world)
    , m_score(new Score())
  {
    setFunction(ElementFunction::PLAYER);
    setFilter(ElementFunction::PLAYER, ElementFunction::ENEMY|ElementFunction::PLAYER|ElementFunction::BOUNDARY);
  }

Player *Player::randomGeneration(b2World *world, Random& m_random) {
  Player *play = nullptr;
  Distribution<unsigned> choice_play = game::Distributions::uniformDistribution (0u, 2u);
  unsigned choice = choice_play(m_random);
  switch (choice) {
  case 0:
    play = new Player(game::ElementType::ROCK, 0.0f, 0.0f, world);
    break;
  case 1:
    play = new Player(game::ElementType::PAPER, 0.0f, 0.0f, world);
    break;
  case 2 :
    play = new Player(game::ElementType::SCISSORS, 0.0f, 0.0f, world);
    break;
  default :
    assert(false);
    break;
  }

  assert(play);
  
  return play;
}
  
  void Player::move(float vx, float vy){
    setLinearVelocity(vx, vy);
    
    return;
  }
  
  Score *Player::getScore(){
    return m_score;
  }
  
}
