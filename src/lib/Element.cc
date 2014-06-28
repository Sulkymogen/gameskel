#include <game/Element.h>
#include <iostream>

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
    fixture.filter.categoryBits = static_cast<uint16>(ElementFunction::ENEMY);
    fixture.filter.maskBits = static_cast<uint16>(ElementFunction::ENEMY|ElementFunction::PLAYER);

    m_body->CreateFixture(&fixture);

    m_body->SetLinearVelocity({ vx, vy });
    m_body->SetUserData(this);
  }
  
  Element::~Element() {
    auto world = m_body->GetWorld();
    world->DestroyBody(m_body);
  }
  
  /*static*/ Element* Element::randomGeneration(b2World *world, Random& m_random) {
    Distribution<unsigned> axis = game::Distributions::uniformDistribution (0u, 3u);
    Distribution<unsigned> type = game::Distributions::uniformDistribution (0u, 9u);
    Distribution<unsigned> coef = game::Distributions::uniformDistribution (50u, 50u + 2 * (int)ELEMENT_SIZE);
    Distribution<float> value = game::Distributions::uniformDistribution (-320.0f - ELEMENT_SIZE, 320.0f + ELEMENT_SIZE);
    Distribution<float> cible = game::Distributions::uniformDistribution (-320.0f, 320.0f);
    unsigned s_axis = axis(m_random);
    unsigned s_type = type(m_random);
    unsigned s_coef = coef(m_random);
    float s_value = value(m_random);
    
    float x = 0.0f;
    float y = 0.0f;
    
    switch (s_axis)
    {
      case 0 :
	x = s_value;
	y = 320.0f + ELEMENT_SIZE;
	break;
      case 1 :
	x = 320.0f + ELEMENT_SIZE;
	y = s_value;
	break;
      case 2 :
	x = s_value;
	y = -320.0f - ELEMENT_SIZE;
	break;
      case 3 :
	x = -320.0f - ELEMENT_SIZE;
	y = s_value;
	break;
      default :
	break;
    }
    
    float cible_x = cible(m_random);
    float cible_y = cible(m_random);
    
    
    float dx = (cible_x - x);
    float dy = (cible_y - y);
    
    float v_coef = s_coef / sqrt(dx*dx+dy*dy);
    
    float vx = v_coef * dx;
    float vy = v_coef * dy;
    
    Element *elt = NULL;    
    
    switch (s_type)
    {
      case 0 :
      case 1 :
	elt = new Element(ElementType::ROCK, x, y, vx, vy, world);
	break;
      case 2 :
      case 3 :
      case 4 :
      case 5 :
      case 6 :
	elt = new Element(game::ElementType::PAPER, x, y, vx, vy, world);
	break;
      case 7 :
      case 8 :
      case 9 :
	elt = new Element(game::ElementType::SCISSORS, x, y, vx, vy, world);
	break;
      default :
	elt = new Element(ElementType::PAPER, x, y, vx, vy, world);
	break;
    }
    
    return elt;
  }

  EntityFuture Element::update(float dt) {
    if (m_state == ElementState::DEAD) {
      return EntityFuture::REMOVE;
    }
    return EntityFuture::KEEP;
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
      shape.setRadius(22.0f);
      shape.setOrigin(6.5f, 6.5f);
      shape.setPosition(pos.x,pos.y);
      shape.setFillColor(sf::Color(0,0,0,180));
      window.draw(shape);
    }

    window.draw(sprite);
  }
  
  void Element::disappear(void)
  {
    m_state = ElementState::DEAD;
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
  
  void Element::setFilter(uint16 categoryBits, uint16 maskBits){
    b2CircleShape circle;
    circle.m_radius = 10.0f;
    
    b2FixtureDef fixture;
    fixture.shape = &circle;
    fixture.density = 1.0f;
    fixture.friction = 0.3f;
    fixture.restitution = 0.9f;
    fixture.filter.categoryBits = categoryBits;
    fixture.filter.maskBits = maskBits;
    
    m_body->CreateFixture(&fixture);
    return;
  }
  
  ElementType Element::getElementType(void) const {
    return m_type;
  }

  sf::Texture * Element::warrior;
    sf::Texture * Element::mother;
    sf::Texture * Element::tiger;
}
