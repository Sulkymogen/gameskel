#ifndef GAME_ELEMENT_H
#define GAME_ELEMENT_H

#include <Box2D/Box2D.h>

namespace game {

  enum class ElementType {
    PAPER,
    ROCK,
    SCISSORS,
  };

  enum class ElementState {
    ALIVE,
    STATE,
  };

  class Element {
  public:
    Element(ElementType type, b2World *world);


  private:
    ElementType m_type;
    ElementState m_state;
    b2Body *m_body;
  };

}

#endif // GAME_ELEMENT_H
