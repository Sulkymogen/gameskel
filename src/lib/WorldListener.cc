#include <game/Element.h>
#include <game/Player.h>
#include <game/WorldListener.h>
#include <iostream>

namespace game {
  
  static void collision(Element * elementPlayer, Element * elementEnnemy) {
    ElementType typeEnnemy = elementEnnemy->getElementType();
    Player * player = static_cast<Player *>(elementPlayer);
    
      switch (player->getElementType()) {
        case ElementType::PAPER:
          if (typeEnnemy == ElementType::ROCK) {
            elementEnnemy->disappear();
            player->getScore()->increaseScore();
          } else if (typeEnnemy == ElementType::SCISSORS) {
            player->getScore()->decreaseScore();
          }
          break;
            
        case ElementType::ROCK:
          if (typeEnnemy == ElementType::SCISSORS) {
            elementEnnemy->disappear();
            player->getScore()->increaseScore();
          } else if (typeEnnemy == ElementType::PAPER) {
            player->getScore()->decreaseScore();
          }
          break;
            
        case ElementType::SCISSORS:
          if (typeEnnemy == ElementType::PAPER) {
            elementEnnemy->disappear();
            player->getScore()->increaseScore();
          } else if (typeEnnemy == ElementType::ROCK) {
            player->getScore()->decreaseScore();
          }
          break;
      }
  }

  void WorldListener::BeginContact(b2Contact* contact) { 
    void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
    void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

    Element * elementA = NULL;
    Element * elementB = NULL;
    
    if (bodyUserDataA) {
      elementA = static_cast<Element *>( bodyUserDataA );
    }
    if (bodyUserDataB) {
      elementB = static_cast<Element *>( bodyUserDataB );
    }
    
    if (bodyUserDataA && bodyUserDataB) {
      if (elementA->isPlayer()) {
        collision(elementA, elementB);        
      } else if (elementB->isPlayer()) {
        collision(elementB, elementA);
      }  else {
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
}