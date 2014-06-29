#ifndef GAME_ELEMENT_H
#define GAME_ELEMENT_H

#include <game/Entity.h>
#include <game/Random.h>
#include <Box2D/Box2D.h>
#include <game/Level.h>

namespace game {
  class World;

  enum class ElementType {
    PAPER,
    ROCK,
    SCISSORS,
    BONUS,
  };

  enum class ElementState {
    ALIVE,
    GHOST,
    DEAD,
  };

  enum ElementFunction{
    PLAYER =		0x0001,
    ENEMY = 		0x0002,
    BOUNDARY = 	 	0x0004,
  };

  class Element : public Entity {
  public:
    Element(ElementType type, float x, float y, float vx, float vy, b2World *world);
    ~Element();

    static Element* randomGeneration(b2World *world, Random& m_random, ElementType player_type, Level * lv);

    virtual EntityFuture update(float dt) override;
    virtual void render(sf::RenderWindow& window) override;
    void disappear(void);
    ElementFunction getFunction (void) const;
    ElementType getElementType (void) const;
    b2Body * getBody (void) const;
    bool isPlayer(void);
    bool isGhost(void) const;
    void setState(ElementState state);

  protected:
    void setFunction (ElementFunction function);
    b2Vec2 getLinearVelocity (void) const;
    void setLinearVelocity (float vx, float vy);
    void setFilter(uint16 categoryBits, uint16 maskBits);

  public:
    static sf::Texture * warrior;
    static sf::Texture * mother;
    static sf::Texture * tiger;
    static sf::Texture * bonus;

  private:
    ElementType m_type;
    ElementState m_state;
    b2Body * m_body;
    ElementFunction m_function;

  public:
    static World *world;
  };

}

#endif // GAME_ELEMENT_H
