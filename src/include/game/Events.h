#ifndef GAME_EVENTS_H
#define GAME_EVENTS_H

#include <game/Event.h>
#include <game/Element.h>

namespace game {

  struct DeadEvent : public Event {
    static const EventType type = "DeadEvent"_type;

    ElementType what;
    b2Vec2 where;
  };
  
  struct CollideEvent : public Event {
    static const EventType type = "CollideEvent"_type;
  };


}





#endif // GAME_EVENTS_H
