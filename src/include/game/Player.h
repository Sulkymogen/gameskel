#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <game/Element.h>

namespace game {

  class Player : public Element
  {
  public:
    explicit Player(ElementType type, float x, float y, b2World *world);

  private:

  };
}

#endif //GAME_PLAYER_H