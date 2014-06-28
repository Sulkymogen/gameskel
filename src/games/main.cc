/*
 * Copyright (c) 2014, Julien Bernard
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <game/World.h>
#include <game/Element.h>
#include <game/Random.h>
#include <game/Resource.h>
#include <game/Player.h>
#include <game/WorldListener.h>

#include <Box2D/Box2D.h>

#include "config.h"

int main() {
  // initialize
  game::Random random;
  
  game::World world;
  sf::RenderWindow window(sf::VideoMode(1024, 768), GAME_NAME " (version " GAME_VERSION ")");
  window.setKeyRepeatEnabled(false);

  sf::View view;
  view.setCenter(0.0f, 0.0f);
  view.setSize(600.0f, 600.0f);
  view.setViewport({ 0, 0, 768.0f / 1024.0f, 1.0f });
  window.setView(view);

  b2Vec2 b2_gravity(0.0f, 0.0f);
  b2World b2_world(b2_gravity);

  int32 velocity_iterations = 10;
  int32 position_iterations = 8;
  
  game::WorldListener worldListenerInstance;
  b2_world.SetContactListener(&worldListenerInstance);
  
  game::Player player(game::ElementType::ROCK, 200.0f, 200.0f, &b2_world);
  world.addEntity(&player, game::Memory::FROM_STACK);
  
  //a static body
  b2BodyDef boundaryDef;
  boundaryDef.type = b2_staticBody;
  boundaryDef.position.Set(0, 0);
  b2Body* staticBody = b2_world.CreateBody(&boundaryDef);

  //shape definition
  b2PolygonShape polygonShape;

  //fixture definition
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &polygonShape;
  fixtureDef.filter.categoryBits = game::ElementFunction::BOUNDARY;
  fixtureDef.filter.maskBits = game::ElementFunction::PLAYER | game::ElementFunction::ENEMY | game::ElementFunction::BOUNDARY;
  
  //add four walls to the static body
  polygonShape.SetAsBox( 310, 10, b2Vec2(0, -310), 0);//ground
  staticBody->CreateFixture(&fixtureDef);
  polygonShape.SetAsBox( 310, 10, b2Vec2(0, 280), 0);//ceiling
  staticBody->CreateFixture(&fixtureDef);
  polygonShape.SetAsBox( 10, 310, b2Vec2(-310, 0), 0);//left wall
  staticBody->CreateFixture(&fixtureDef);
  polygonShape.SetAsBox( 10, 310, b2Vec2(280, 0), 0);//right wall
  staticBody->CreateFixture(&fixtureDef);

  // load resources



  game::ResourceManager manager;

  manager.addSearchDir(GAME_DATADIR);

  game::Element::warrior=manager.getTexture("warrior.png");
  game::Element::warrior->setSmooth(true);
  game::Element::tiger=manager.getTexture("tiger.png");
  game::Element::tiger->setSmooth(true);
  game::Element::mother=manager.getTexture("mother.png");
  game::Element::mother->setSmooth(true);

  sf::Texture * background = manager.getTexture("background.jpg");
  sf::Sprite bgsprite ;
  bgsprite.setScale(0.66f,1.0f);
  bgsprite.setPosition(-300,-300);
  bgsprite.setTexture(* background);

  // add entities
  //game::Element elt(game::ElementType::PAPER, 0.0f, 0.0f, 50.0f, 0.0f, &b2_world);
  //world.addEntity(&elt, game::Memory::FROM_STACK);

  //game::Element elt2(game::ElementType::ROCK, 100.0f, 0.0f, -100.0f, 0.0f, &b2_world);
  //world.addEntity(&elt2, game::Memory::FROM_STACK);

  //game::Element elt3(game::ElementType::SCISSORS, -100.0f, 0.0f, 200.0f, 0.0f, &b2_world);
  //world.addEntity(&elt3, game::Memory::FROM_STACK);

  game::Element *elmt;
  
  for (int i = 0; i < 15; i++)
  {
    elmt = game::Element::randomGeneration(&b2_world, random);
    world.addEntity(elmt, game::Memory::FROM_HEAP);
  }
  
  // main loop
  sf::Clock clock;
  while (window.isOpen()) {
    // input
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::KeyPressed) {

        switch (event.key.code) {
          case sf::Keyboard::Escape:
            window.close();
            break;
	    
	  case sf::Keyboard::Up:
	    player.move(game::PlayerMove::UP);
	    break;
	    
	  case sf::Keyboard::Left:
	    player.move(game::PlayerMove::LEFT);
	    break;
	    
	  case sf::Keyboard::Down:
	    player.move(game::PlayerMove::BOTTOM);
	    break;
	  
	  case sf::Keyboard::Right:
	    player.move(game::PlayerMove::RIGHT);
	    break;

          default:
            break;
        }
      }
      else if (event.type == sf::Event::KeyReleased) {

	switch (event.key.code) {    
	  case sf::Keyboard::Up:
	    player.stop(game::PlayerMove::UP);
	    break;
	    
	  case sf::Keyboard::Left:
	    player.stop(game::PlayerMove::LEFT);
	    break;
	    
	  case sf::Keyboard::Down:
	    player.stop(game::PlayerMove::BOTTOM);
	    break;
	  
	  case sf::Keyboard::Right:
	    player.stop(game::PlayerMove::RIGHT);
	    break;

          default:
            break;
	}
      }
    }

    // update
    sf::Time elapsed = clock.restart();
    float dt = elapsed.asSeconds();
    b2_world.Step(dt, velocity_iterations, position_iterations);
    world.update(dt);

    // render
    window.clear(sf::Color::White);
    window.draw(bgsprite);
    world.render(window);
    window.display();
  }

  return 0;
}
