#ifndef GAME_ELEMENT_H
#define GAME_ELEMENT_H

#include <game/Entity.h>
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
  
  enum ElementFunction{
    PLAYER =		0x0001,
    ENEMY = 		0x0002,
    BOUNDARY = 	 	0x0004,
  };

  class Element : public Entity {
  public:
    Element(ElementType type, float x, float y, float vx, float vy, b2World *world);

    virtual void update(float dt) override;
    virtual void render(sf::RenderWindow& window) override;
    
  protected:
    ElementFunction getFunction(void) const;
    void setFunction(ElementFunction function);
    b2Vec2 getLinearVelocity(void) const;
    void setLinearVelocity(float vx, float vy);
    void setFilter(uint16 categoryBits, uint16 maskBits);

  private:
    ElementType m_type;
    ElementState m_state;
    b2Body *m_body;
    ElementFunction m_function;
    
  };

}

#endif // GAME_ELEMENT_H
