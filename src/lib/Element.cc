#include <game/Element.h>

#define ELEMENT_SIZE 20.0f

namespace game {

  Element::Element(ElementType type, float x, float y, float vx, float vy, b2World *world)
  : m_type(type)
  , m_state(ElementState::ALIVE)
  , m_body(nullptr)
  , m_function(ElementFunction::ENEMY)
  {
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(x, y);

    m_body = world->CreateBody(&def);

    b2CircleShape circle;

    circle.m_radius = ELEMENT_SIZE;

    b2FixtureDef fixture;
    fixture.shape = &circle;
    fixture.density = 1.0f;
    fixture.friction = 0.3f;
    fixture.restitution = 0.9f;

    m_body->CreateFixture(&fixture);

    m_body->SetLinearVelocity({ vx, vy });
  }

  void Element::update(float dt) {

  }

  void Element::render(sf::RenderWindow& window) {
    auto pos = m_body->GetPosition();

    sf::Sprite sprite;
    sprite.setScale(ELEMENT_SIZE/90,ELEMENT_SIZE/90);
    sprite.setOrigin(ELEMENT_SIZE, ELEMENT_SIZE);
    sprite.setPosition(pos.x, pos.y);

    switch(m_type){
    case(ElementType::PAPER):
      sprite.setTexture(*warrior);
      break;
    case(ElementType::ROCK):
      sprite.setTexture(*tiger);
      break;
    case(ElementType::SCISSORS):
      sprite.setTexture(*mother);
      break;
    }

    window.draw(sprite);

    if (m_function == ElementFunction::PLAYER) {
      sf::CircleShape shape;
      shape.setRadius(5.0f);
      shape.setOrigin(5.0f, 5.0f);
      shape.setPosition(pos.x,pos.y);
      shape.setFillColor(sf::Color::Black);
      window.draw(shape);
    }

    window.draw(sprite);
  }

  ElementFunction Element::getFunction(void) const{
    return m_function;
  }

  void Element::setFunction(ElementFunction function){
    m_function = function;

    return;
  }

  b2Vec2 Element::getLinearVelocity(void) const{
    return m_body->GetLinearVelocity();
  }

  void Element::setLinearVelocity(float vx, float vy){
    m_body->SetLinearVelocity({vx, vy});

    return;
  }

  sf::Texture * Element::warrior;
    sf::Texture * Element::mother;
    sf::Texture * Element::tiger;
}
