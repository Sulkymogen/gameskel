#ifndef GAME_SCORE_H
#define GAME_SCORE_H

#include <game/Entity.h>

namespace game {
  
  class Score : public Entity
  {
  public:
    explicit Score();
    
    void setFont(sf::Font *font);
    
    int getScore() const;
    
    void increaseScore(void);
    void increaseScore(int value);
    void decreaseScore(void);
    
    virtual void render(sf::RenderWindow& window);
    
  private:
    enum class ScoreStatus{
      UP,
      IDDLE,
      DOWN,
    };
      
    int m_score;
    sf::Font *m_font;
    ScoreStatus m_upScore;
    int m_factor;
    int m_scoreMax;
  };
}

#endif //GAME_SCORE_H