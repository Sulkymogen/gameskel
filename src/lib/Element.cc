#include <game/Element.h>
#include <iostream>

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
  
  /*static*/ Element* Element::randomGeneration(b2World *world, Random& m_random) {
    Distribution<unsigned> axis = game::Distributions::uniformDistribution (0u, 3u);
    Distribution<unsigned> type = game::Distributions::uniformDistribution (0u, 2u);
    Distribution<float> value = game::Distributions::uniformDistribution (-330.0f, 330.0f);
    Distribution<float> cible = game::Distributions::uniformDistribution (-320.0f, 320.0f);
    unsigned s_axis = axis(m_random);
    unsigned s_type = type(m_random);
    float s_value = value(m_random);
    
    float x = 0.0f;
    float y = 0.0f;
    
    switch (s_axis)
    {
      case 0 :
	x = s_value;
	y = 330.0f;
	break;
      case 1 :
	x = 330.0f;
	y = s_value;
	break;
      case 2 :
	x = -s_value;
	y = -330.0f;
	break;
      case 3 :
	x = -330.0f;
	y = -s_value;
	break;
      default :
	break;
    }
    
    float cible_x = cible(m_random);
    float cible_y = cible(m_random);
    
    
    float dx = (cible_x - x);
    float dy = (cible_y - y);
    
    float vx = dx/20.0f;//(dx*dx+dy*dy);
    float vy = dy/20.0f;//(dx*dx+dy*dy);
    
    std::cout << "Position X : " << x << std::endl;
    std::cout << "Position Y : " << y << std::endl;
    std::cout << "Vitesse X : " << vx << std::endl;
    std::cout << "Vitesse Y : " << vy << std::endl;
    
    Element *elt = NULL;
    
    switch (s_type)
    {
      case 0 :
	elt = new Element(game::ElementType::PAPER, x, y, vx, vy, world);
	break;
      case 1 :
	elt = new Element(game::ElementType::ROCK, x, y, vx, vy, world);
	break;
      case 2 :
	elt = new Element(game::ElementType::SCISSORS, x, y, vx, vy, world);
	break;
      default :
	elt = new Element(game::ElementType::PAPER, x, y, vx, vy, world);
	break;
    }
    
    return elt;
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
