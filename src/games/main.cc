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
#include <game/Param.h>
#include <iostream>

#include <Box2D/Box2D.h>

#include <iostream>

#include "config.h"

#define ENTITIES_NUMBER 15

int main() {
  // initialize
  std::random_device dev;
  game::Random random(dev());

  b2Vec2 b2_gravity(0.0f, 0.0f);
  b2World b2_world(b2_gravity);

  game::World world;
  sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), GAME_NAME " (version " GAME_VERSION ")", sf::Style::Titlebar|sf::Style::Close);
  window.setKeyRepeatEnabled(false);

  float ratio = static_cast<float>(SCREEN_HEIGHT) / static_cast<float>(SCREEN_WIDTH);

  sf::View main_view;
  main_view.setCenter(0.0f, 0.0f);
  main_view.setSize(600.0f, 600.0f);
  main_view.setViewport({ 0, 0, ratio, 1.0f });

  sf::View secondary_view({ 0, 0, SCREEN_WIDTH - SCREEN_HEIGHT, SCREEN_HEIGHT });
  secondary_view.setViewport({ ratio, 0, 1.0f - ratio, 1.0f });

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
  bgsprite.setScale(0.59,0.79f);
  bgsprite.setPosition(-300,-300);
  bgsprite.setTexture(* background);

  sf::Font *font = manager.getFont("arial.ttf");
  player.getScore()->setFont(font);

  game::Element *elmt;

  for (int i = 0; i < ENTITIES_NUMBER; i++)
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
      if (ENTITIES_NUMBER + 1 > b2_world.GetBodyCount())
      {
	elmt = game::Element::randomGeneration(&b2_world, random);
	world.addEntity(elmt, game::Memory::FROM_HEAP);
      }

      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::KeyPressed) {

        switch (event.key.code) {
          case sf::Keyboard::Escape:
            window.close();
            break;

          default:
            break;
        }
      }

      // clear when out of screen
      int i = 0;
      b2Body * currentBody = b2_world.GetBodyList();

      while (i < b2_world.GetBodyCount())
      {
	b2Vec2 pos = currentBody->GetPosition();
	if (pos.x < -340 || pos.x > 340 || pos.y < -340 || pos.y > 340)
	{
	  void* bodyUserData = currentBody->GetUserData();
	  game::Element * element;
	  if (bodyUserData) {
	    element = static_cast<game::Element *>( bodyUserData);
	    element->disappear();
	  }
	}

	currentBody = currentBody->GetNext();
	i++;
      }
    }

    float vx = 0.0f;
    float vy = 0.0f;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
      vy -= PLAYER_SPEED;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
      vy += PLAYER_SPEED;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
      vx -= PLAYER_SPEED;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
      vx += PLAYER_SPEED;
    }

    float vmax = sqrt(vx*vx+vy*vy);
    if (0 != vmax)
    {
      vx = PLAYER_SPEED * vx/vmax;
      vy = PLAYER_SPEED * vy/vmax;
    }

    player.move(vx, vy);

    // update
    sf::Time elapsed = clock.restart();
    float dt = elapsed.asSeconds();
    b2_world.Step(dt, velocity_iterations, position_iterations);
    world.update(dt);

    // render main view
    window.clear(sf::Color::White);
    window.setView(main_view);
    window.draw(bgsprite);
    world.render(window);

    //Render secondary view
    window.setView(secondary_view);
    player.getScore()->render(window);


    window.display();
  }

  return 0;
}
