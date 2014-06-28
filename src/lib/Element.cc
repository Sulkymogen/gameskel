#include <game/Element.h>

namespace game {

  Element::Element(ElementType type, b2World *world)
  : m_type(type)
  , m_state(ElementState::ALIVE)
  , m_body(nullptr)
  {

  }

}
