#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#define PLAYER_SPEED 50.0f

#include <game/Element.h>
#include <game/Score.h>

namespace game {

  class Player : public Element
  {
  public:
    explicit Player(ElementType type, float x, float y, b2World *world);
    
    void move(float vx, float vy);
    
    Score *getScore();

  private:
    Score *m_score;
  };
}

#endif //GAME_PLAYER_H