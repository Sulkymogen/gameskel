#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <game/Element.h>

namespace game {
  
  enum class PlayerMove{
    UP,
    LEFT,
    BOTTOM,
    RIGHT,
    STOP,
  };

  class Player : public Element
  {
  public:
    explicit Player(ElementType type, float x, float y, b2World *world);
    
    void move(PlayerMove move);

  private:

  };
}

#endif //GAME_PLAYER_H