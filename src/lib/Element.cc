#include <game/Element.h>
#include <game/Bonus.h>
#include <cassert>
#include <cmath>

#include <game/Events.h>
#include <game/Param.h>
#include <game/World.h>

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
    fixture.friction = 0.05f;
    fixture.restitution = 1.0f;
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

  Element *Element::randomGeneration(b2World *world, Random& m_random, ElementType player_type, Level * lv) {
    Distribution<unsigned> axis_dist = game::Distributions::uniformDistribution (0u, 3u);
    Distribution<unsigned> type_dist = game::Distributions::uniformDistribution (0u, 19u);
    Distribution<unsigned> coef_dist = game::Distributions::uniformDistribution (5u, 5u + 2 * (int)ELEMENT_SIZE);
    Distribution<float> value_dist = game::Distributions::uniformDistribution (-32.0f - ELEMENT_SIZE, 32.0f + ELEMENT_SIZE);
    Distribution<float> cible_dist = game::Distributions::uniformDistribution (-32.0f, 32.0f);
    unsigned axis = axis_dist(m_random);
    unsigned type = type_dist(m_random);
    unsigned coef = coef_dist(m_random);
    float value = value_dist(m_random);

    float x = 0.0f;
    float y = 0.0f;

    switch (axis) {
    case 0:
      x = value;
      y = 32.0f + ELEMENT_SIZE;
      break;
    case 1:
      x = 32.0f + ELEMENT_SIZE;
      y = value;
      break;
    case 2:
      x = value;
      y = -32.0f - ELEMENT_SIZE;
      break;
    case 3:
      x = -32.0f - ELEMENT_SIZE;
      y = value;
      break;
    default:
      assert(false);
      break;
    }

    float cible_x = cible_dist(m_random);
    float cible_y = cible_dist(m_random);

    float dx = (cible_x - x);
    float dy = (cible_y - y);

    float v_coef = coef / std::sqrt(dx*dx+dy*dy);

    float vx = v_coef * dx * (lv->getLevel() * 0.2f + 1);
    float vy = v_coef * dy * (lv->getLevel() * 0.2f + 1);

    Element *elt = nullptr;

    ElementType hunter;
    ElementType target;

    switch(player_type){
    case (ElementType::ROCK) :
      target = ElementType::SCISSORS;
      hunter = ElementType::PAPER;
      break;
    case (ElementType::PAPER) :
      target = ElementType::ROCK;
      hunter = ElementType::SCISSORS;
      break;
    case (ElementType::SCISSORS) :
      target = ElementType::PAPER;
      hunter = ElementType::ROCK;
      break;
    default :
      target = player_type;
      hunter = player_type;
    }



    switch (type) {
    case 0:
    case 1:
    case 2:
    case 3:
      elt = new Element(player_type, x, y, vx, vy, world);
      break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
      elt = new Element(hunter, x, y, vx, vy, world);
      break;
    case 14 :
    case 15 :
    case 16 :
    case 17:
    case 18:
      elt = new Element(target, x, y, vx, vy, world);
      break;
    case 19:
      elt = new Bonus(x, y, vx, vy, world);
      break;
    default :
      assert(false);
      break;
    }

    assert(elt);

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
    auto angle = m_body->GetAngle();

    sf::Sprite sprite;
    sprite.setOrigin(45 * ELEMENT_SIZE, 45 * ELEMENT_SIZE);
    sprite.setScale(ELEMENT_SIZE / 90.0f, ELEMENT_SIZE / 90.0f);
    sprite.setPosition(pos.x, pos.y);
    sprite.setRotation(angle * 180 / M_PI);

    switch(m_type){
    case ElementType::PAPER:
      sprite.setTexture(*warrior);
      break;
    case ElementType::ROCK:
      sprite.setTexture(*tiger);
      break;
    case ElementType::SCISSORS:
      sprite.setTexture(*mother);
      break;
    case ElementType::BONUS:
      sprite.setTexture(*bonus);
      break;
    }

    if (m_function == ElementFunction::PLAYER) {
      sf::CircleShape shape;
      shape.setRadius(2.4f);
      shape.setOrigin(2.4f, 2.4f);
      shape.setPosition(pos.x,pos.y);

      sf::Color color;
      switch(m_type){
      case game::ElementType::PAPER: // warrior
	color = sf::Color(0x00, 0x80, 0xFF);
	break;
      case game::ElementType::ROCK: // tiger
	color = sf::Color(0x03A, 0xDF, 0x00);
	break;
      case game::ElementType::SCISSORS: // mother
	color = sf::Color(0xFF, 0x40, 0x00);
	break;
      default:
	break;
      }
      shape.setFillColor(color);
      window.draw(shape);
    }

    window.draw(sprite);
  }

  void Element::disappear() {
    m_state = ElementState::DEAD;

    assert(world);

    DeadEvent event;
    event.what = m_type;
    event.where = m_body->GetPosition();
    world->triggerEvent(nullptr, &event);
  }

  ElementFunction Element::getFunction() const{
    return m_function;
  }

  bool Element::isPlayer() {
    if (m_function == ElementFunction::PLAYER) {
      return true;
    }
    return false;
  }

  void Element::setFunction(ElementFunction function) {
    m_function = function;
  }

  b2Vec2 Element::getLinearVelocity(void) const{
    return m_body->GetLinearVelocity();
  }

  void Element::setLinearVelocity(float vx, float vy){
    m_body->SetLinearVelocity({vx, vy});
  }

  void Element::setFilter(uint16 categoryBits, uint16 maskBits){
    for(b2Fixture *fixture = m_body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
      b2Filter filter = fixture->GetFilterData();

      filter.categoryBits = categoryBits;
      filter.maskBits = maskBits;

      fixture->SetFilterData(filter);
    }
  }

  ElementType Element::getElementType(void) const {
    return m_type;
  }

  b2Body * Element::getBody (void) const {
    return m_body;
  }

  bool Element::isGhost(void) const {
    return ElementState::GHOST == m_state;
  }

  void Element::setState(ElementState state) {
    m_state = state;
  }

  sf::Texture * Element::warrior;
  sf::Texture * Element::mother;
  sf::Texture * Element::tiger;
  sf::Texture * Element::bonus;

  World *Element::world = nullptr;

}
