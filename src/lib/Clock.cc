#include <game/Clock.h>

namespace game{

  Clock::Clock()
  : m_timer(0.0f){
    
  }
  
  EntityFuture Clock::update(float dt){
    m_timer += dt;
    
    return EntityFuture::KEEP;
  }
  
  void Clock::render(sf::RenderWindow& window){
    sf::Text text;
    text.setFont(*m_font);
    text.setString("Time: "+std::to_string(static_cast<int>(m_timer)));
    text.setCharacterSize(24);
    text.setPosition(60, 250);
    text.setColor(sf::Color::Black);
    
    window.draw(text);
  }
  
  void Clock::setFont(sf::Font *font){
    m_font = font;
  }
  
  int Clock::getTimeElapsed() const{
    return static_cast<int>(m_timer);
  }

}
