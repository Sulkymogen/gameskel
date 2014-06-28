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
    fixture.restitution = 0.9f;

    m_body->CreateFixture(&fixture);

    m_body->SetLinearVelocity({ vx, vy });
  }
  
  void randomGeneration(b2World *world, Random& m_random) {
    Distribution<unsigned> m_axis;
    Distribution<float> m_value;
    Distribution<float> m_cible;
    unsigned axis = m_axis(m_random);
    float value = m_value(m_random);
    
    float x = 0.0f;
    float y = 0.0f;
    
    switch (axis)
    {
      case 0 :
	x = value;
	y = 330.0f;
	break;
      case 1 :
	x = 330.0f;
	y = value;
	break;
      case 2 :
	x = -value;
	y = -330.0f;
	break;
      case 3 :
	x = -330.0f;
	y = -value;
	break;
      default :
	break;
    }
    
    float cible_x = m_cible(m_random);
    float cible_y = m_cible(m_random);
    
    float dx = (cible_x - x);
    float dy = (cible_y - y);
    
    game::Element elt(game::ElementType::PAPER, x, y, dx/(dx*dx+dy*dy), dy/(dx*dx+dy*dy), world);
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
