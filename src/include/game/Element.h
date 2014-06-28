#ifndef GAME_ELEMENT_H
#define GAME_ELEMENT_H

#include <game/Entity.h>
#include <game/Random.h>
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

  class Element : public Entity {
  public:
    Element(ElementType type, float x, float y, float vx, float vy, b2World *world);
    
    void randomGeneration(b2World *world, Random& m_random);

    virtual void update(float dt) override;
    virtual void render(sf::RenderWindow& window) override;

  private:
    ElementType m_type;
    ElementState m_state;
    b2Body *m_body;
  };

}

#endif // GAME_ELEMENT_H
