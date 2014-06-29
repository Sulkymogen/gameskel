#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include <game/Entity.h>

namespace game {
  
  class Level : public Entity
  {
  public:
    explicit Level();
    
    void setFont(sf::Font *font);
    
    int getLevel() const;
    
    void increaseLevel(void);
    void decreaseLevel(void);
    void resetLevel(void);
    void setLevel(int l);
    
    virtual void render(sf::RenderWindow& window);
    
  private:
    int m_level;
    sf::Font *m_font;
  };
}

#endif //GAME_LEVEL_H
