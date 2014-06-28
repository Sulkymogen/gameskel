#include <game/Element.h>
#include <game/WorldListener.h>
#include <iostream>

namespace game {

  void WorldListener::BeginContact(b2Contact* contact) { 
    void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
    void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

    Element * elementA, * elementB;
    std::cout << "coucou\n";
    if (bodyUserDataA) {
      elementA = static_cast<Element *>( bodyUserDataA );
    }
    if (bodyUserDataB) {
      elementB = static_cast<Element *>( bodyUserDataB );
    }
    
    if (bodyUserDataA && bodyUserDataB) {
      elementA->disappear();
      elementB->disappear();
      /*
      switch (elementA->getElementType())
      {
	case PAPER:
	  elementA->disappear();
	  elementB->disappear();
	  break;
	  
	case ROCK:
	  elementA->disappear();
	  elementB->disappear();
	  break;
	  
	case SCISSORS:
	  elementA->disappear();
	  elementB->disappear();
	  break;
	
      }
      */
    }
    
  }
}