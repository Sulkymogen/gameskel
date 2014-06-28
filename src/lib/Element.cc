#include <game/Element.h>

namespace game {

  Element::Element(ElementType type, float x, float y, float vx, float vy, b2World *world)
  : m_type(type)
  , m_state(ElementState::ALIVE)
  , m_body(nullptr)
  {
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(x, y);

    m_body = world->CreateBody(&def);

    b2CircleShape circle;

    circle.m_radius = 10.0f;

    b2FixtureDef fixture;
    fixture.shape = &circle;
    fixture.density = 1.0f;
    fixture.friction = 0.3f;

    m_body->CreateFixture(&fixture);

    m_body->SetLinearVelocity({ vx, vy });
  }

  void Element::update(float dt) {

  }

  void Element::render(sf::RenderWindow& window) {
    auto pos = m_body->GetPosition();

    sf::CircleShape shape(10.0f);
    shape.setOrigin(10.0f, 10.0f);
    shape.setPosition(pos.x, pos.y);

    switch(m_type){
    case(ElementType::PAPER): 
      shape.setFillColor(sf::Color::Red);
      break;
    case(ElementType::ROCK):
      shape.setFillColor(sf::Color::Green);
      break;
    case(ElementType::SCISSORS):
      shape.setFillColor(sf::Color::Blue);
      break;
    default : 
      shape.setFillColor(sf::Color::Yellow);
    }

    window.draw(shape);
  }


}
