#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#define PLAYER_SPEED 50.0f

#include <game/Element.h>

namespace game {
  
  enum class PlayerMove{
    UP,
    LEFT,
    BOTTOM,
    RIGHT,
  };

  class Player : public Element
  {
  public:
    explicit Player(ElementType type, float x, float y, b2World *world);
    
    void move(PlayerMove move);
    void stop(PlayerMove move);

  private:

  };
}

#endif //GAME_PLAYER_H