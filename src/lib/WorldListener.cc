#include <game/Element.h>
#include <game/Player.h>
#include <game/WorldListener.h>
#include <iostream>

namespace game {

  void WorldListener::BeginContact(b2Contact* contact) { 
    void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
    void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

    Element * elementA, * elementB;
    if (bodyUserDataA) {
      elementA = static_cast<Element *>( bodyUserDataA );
    }
    if (bodyUserDataB) {
      elementB = static_cast<Element *>( bodyUserDataB );
    }
    
    if (bodyUserDataA && bodyUserDataB) {
      ElementType typeB = elementB->getElementType();
    
      switch (elementA->getElementType()) {
        case ElementType::PAPER:
          if (typeB == ElementType::ROCK) {
            elementB->disappear();
          } else if (typeB == ElementType::SCISSORS) {
            elementA->disappear();
          }
          break;
            
        case ElementType::ROCK:
          if (typeB == ElementType::SCISSORS) {
            elementB->disappear();
          } else if (typeB == ElementType::PAPER) {
            elementA->disappear();
          }
          break;
            
        case ElementType::SCISSORS:
          if (typeB == ElementType::PAPER) {
            elementB->disappear();
          } else if (typeB == ElementType::ROCK) {
            elementA->disappear();
          }
          break;
        
      }
      
    }
    
  }
}