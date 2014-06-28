#include <game/Score.h>
#include <iostream>

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
    sf::CircleShape shape;
    shape.setRadius(22.0f);
    shape.setOrigin(6.5f, 6.5f);
    shape.setPosition(0,0);
    shape.setFillColor(sf::Color::Black);
    window.draw(shape);
    
    std::cout << "render score\n";
  }
  
}