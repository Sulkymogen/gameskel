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
    DEAD,
  };
  
  enum class ElementFunction {
    PLAYER,
    ENEMY,
  };

  class Element : public Entity {
  public:
    Element(ElementType type, float x, float y, float vx, float vy, b2World *world);
    ~Element();
    
    static Element* randomGeneration(b2World *world, Random& m_random);

    virtual EntityFuture update(float dt) override;
    virtual void render(sf::RenderWindow& window) override;
    void disappear(void);
    ElementFunction getFunction (void) const;
    ElementType getElementType (void) const;
    
    
  protected:
    
    void setFunction (ElementFunction function);
    b2Vec2 getLinearVelocity (void) const;
    void setLinearVelocity (float vx, float vy);

  public:
    static sf::Texture * warrior; 
    static sf::Texture * mother;
    static sf::Texture * tiger;

  private:
    ElementType m_type;
    ElementState m_state;
    b2Body * m_body;
    ElementFunction m_function;
    
  };

}

#endif // GAME_ELEMENT_H
