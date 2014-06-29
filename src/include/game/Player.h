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

    static Player* randomGeneration(b2World *world, Random& m_random);

    void move(float vx, float vy);
    
    Score *getScore();
    Level *getLevel();

  private:
    Score *m_score;
    Level *m_level;
  };
}

#endif //GAME_PLAYER_H
