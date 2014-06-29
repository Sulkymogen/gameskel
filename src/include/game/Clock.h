#ifndef GAME_CLOCK_H
#define GAME_CLOCK_H

#include <game/Entity.h>

namespace game{

  class Clock : public Entity{
  public:
    explicit Clock();
    
    void setFont(sf::Font *font);
    
    virtual EntityFuture update(float dt);
    virtual void render(sf::RenderWindow& window);
    
  private:
    float m_timer;
    sf::Font *m_font;
  };

}

#endif // GAME_CLOCK_H