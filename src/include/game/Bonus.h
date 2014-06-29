#ifndef GAME_BONUS_H
#define GAME_BONUS_H

#include <game/Element.h>
#include <game/Score.h>
#include <game/Param.h>

namespace game {

  class Bonus : public Element
  {
  public:
    explicit Bonus(float x, float y, float vx, float vy, b2World *world);


  private:
    int value;
  };
}

#endif //GAME_BONUS_H
