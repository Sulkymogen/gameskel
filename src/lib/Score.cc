#include <game/Score.h>

namespace game {
  Score::Score() :
  m_score(0)
  , m_upScore(ScoreStatus::IDDLE)
  , m_factor(1)
  , m_scoreMax(0){
    
  }
  
  int Score::getScore() const{
    return m_score;
  }
  
  void Score::increaseScore(void){
    //si on enchaine des ennemies
    if(m_upScore == ScoreStatus::UP){
      m_factor++;
    }
    else{
      m_upScore = ScoreStatus::UP;
      m_factor = 1;
    }
    
     m_score+= m_factor*100;
     
     if(m_scoreMax < m_score){
       m_scoreMax = m_score;
     }
  }
  
  void Score::increaseScore(int value){
    //si on enchaine des ennemies
    if(m_upScore == ScoreStatus::UP){
      m_factor++;
    }
    else{
      m_upScore = ScoreStatus::UP;
      m_factor = 1;
    }
    
     m_score+= m_factor*value;
     
     if(m_scoreMax < m_score){
       m_scoreMax = m_score;
     }
  }
  
  void Score::decreaseScore(void){
    //si on enchaine des ennemies
    if(m_upScore == ScoreStatus::DOWN){
      m_factor++;
    }
    else{
      m_upScore = ScoreStatus::DOWN;
      m_factor = 1;
    }
    
    m_score-= m_factor*500;
  }
  
  
  void Score::render(sf::RenderWindow& window){
    sf::Text text;
    
    text.setFont(*m_font);
    text.setString("Score Max: "+std::to_string(m_scoreMax));
    text.setCharacterSize(24);
    text.setColor(sf::Color(0xdf, 0x01, 0x01));
    text.setPosition(60, 160);
    
    window.draw(text);
    
    text.setFont(*m_font);
    text.setString("Score: "+std::to_string(m_score));
    text.setCharacterSize(24);
    text.setColor(sf::Color::Black);
    text.setPosition(60, 190);
    
    window.draw(text);
    
    if(m_upScore == ScoreStatus::UP || m_upScore == ScoreStatus::IDDLE)
      text.setString("Combo: x"+std::to_string(m_factor));
    else
      text.setString("Combo: x-"+std::to_string(m_factor));
    
    //text.setPosition(0, 90);
    text.setPosition(60, 280);
    window.draw(text);
  }
  
  void Score::setFont(sf::Font *font){
    m_font = font;
  }
}
