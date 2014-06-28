#include <game/Score.h>

namespace game {
  Score::Score() :
  m_score(0){
    
  }
  
  int Score::getScore() const{
    return m_score;
  }
  
  void Score::increaseScore(void){
    m_score+= 100;
    return;
  }
  
  void Score::decreaseScore(void){
    m_score-= 100;
    return;
  }
  
  
  void Score::render(sf::RenderWindow& window){
    sf::Text text;
    text.setFont(*m_font);
    text.setString("Score: "+std::to_string(m_score));
    text.setCharacterSize(24);
    text.setColor(sf::Color::Black);
    
    window.draw(text);
  }
  
  void Score::setFont(sf::Font *font){
    m_font = font;
  }
}