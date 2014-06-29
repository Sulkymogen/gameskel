#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <game/Element.h>
#include <game/Score.h>
#include <game/Param.h>

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