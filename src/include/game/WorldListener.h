#ifndef WORLD_LISTENER_H
#define WORLD_LISTENER_H

#include <Box2D/Box2D.h>
#include <game/Resource.h>

namespace game {

  class WorldListener : public b2ContactListener {
  
  public:
      void BeginContact(b2Contact* contact);
      
  
  };
}
#endif // WORLD_LISTENER_H