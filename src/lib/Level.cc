#include <game/Level.h>

namespace game {
  Level::Level() :
    m_level(1){
  }
  
  int Level::getLevel() const{
    return m_level;
  }
  
  void Level::increaseLevel(void){
    m_level+= 1;
    return;
  }
  
  void Level::decreaseLevel(void){
    m_level-= 1;
    return;
  }

  void Level::resetLevel(void){
    m_level-= 0;
    return;
  }

  void Level::setLevel(int l){
    if(l<1)
      l=1;
    m_level= l;
    return;
  }
  
  
  void Level::render(sf::RenderWindow& window){
    sf::Text text;
    text.setFont(*m_font);
    text.setString("Level: "+std::to_string(m_level));
    text.setCharacterSize(24);
    text.setColor(sf::Color::Black);
    text.setPosition(0,30);
    
    window.draw(text);
  }
  
  void Level::setFont(sf::Font *font){
    m_font = font;
  }
}
