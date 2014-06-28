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
    void decreaseScore(void);
    
    virtual void render(sf::RenderWindow& window);
    
  private:
    int m_score;
    sf::Font *m_font;
  };
}

#endif //GAME_SCORE_H