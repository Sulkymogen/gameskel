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
#include <iostream>

#include <game/World.h>
#include <game/Element.h>
#include <game/Random.h>
#include <game/Resource.h>
#include <game/Player.h>
#include <game/WorldListener.h>
#include <game/Param.h>
#include <game/Events.h>
#include <game/Particle.h>
#include <game/Clock.h>

#include <Box2D/Box2D.h>

#include "config.h"

#include <SFML/Audio.hpp>

int main() {
  
  game::ResourceManager manager;

  manager.addSearchDir(GAME_DATADIR);

  sf::SoundBuffer * ambiance_buffer = manager.getSoundBuffer("ambiance.ogg");
  sf::Sound ambiance;
  ambiance.setBuffer(* ambiance_buffer);
  ambiance.setLoop(true);
  ambiance.setVolume(100.0f);
  ambiance.play();
  
  sf::SoundBuffer * death_buffer = manager.getSoundBuffer("nutfall.flac");
  sf::Sound death;
  death.setVolume(40.0f);
  death.setBuffer(* death_buffer);
  
  
  //
  // Menu
  sf::RenderWindow menu(sf::VideoMode(MENU_WIDTH, MENU_HEIGHT), "The Game With No Name (version " GAME_VERSION ")", sf::Style::Titlebar|sf::Style::Close);
  menu.setKeyRepeatEnabled(false);

  sf::Texture * menu_bg = manager.getTexture("menu_bg2.png");
  sf::Sprite menu_bg_sprite;
  menu_bg_sprite.setPosition(0, 0);
  menu_bg_sprite.setTexture(* menu_bg);


  sf::Texture * menu_logo = manager.getTexture("guerrier-tigre-maman-transparent.png");
  sf::Sprite menu_logo_sprite;
  menu_logo_sprite.setPosition(10, 0);
  menu_logo_sprite.setScale(0.40f, 0.40f);
  menu_logo_sprite.setTexture(* menu_logo);
  
  sf::Texture * menu_name = manager.getTexture("game_name_penguin.png");
  sf::Sprite menu_name_sprite;
  menu_name_sprite.setPosition(25, 285);
  menu_name_sprite.setScale(0.90f, 0.90f);
  menu_name_sprite.setTexture(* menu_name);
  
  sf::Texture * menu_jouer = manager.getTexture("jouer2_2.png");
  sf::Sprite menu_jouer_sprite;
  menu_jouer_sprite.setPosition(130, 490);
  menu_jouer_sprite.setScale(0.80f, 0.80f);
  menu_jouer_sprite.setTexture(* menu_jouer);

  sf::Texture * menu_quitter = manager.getTexture("quitter2.png");
  sf::Sprite menu_quitter_sprite;
  menu_quitter_sprite.setPosition(130, 580);
  menu_quitter_sprite.setScale(0.80f, 0.80f);
  menu_quitter_sprite.setTexture(* menu_quitter);

  bool play = false;

  while (menu.isOpen()) {
    // input
    sf::Event event;
    while (menu.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        menu.close();
      } else if (event.type == sf::Event::KeyPressed) {

        switch (event.key.code) {
          case sf::Keyboard::Escape:
            menu.close();
            break;

          default:
            break;
        }

      } else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

        const sf::Vector2i localPosition = sf::Mouse::getPosition(menu);

        if (menu_jouer_sprite.getGlobalBounds().contains(localPosition.x, localPosition.y)) {
          play = true;
          menu.close();
        } else if (menu_quitter_sprite.getGlobalBounds().contains(localPosition.x, localPosition.y)) {
          menu.close();
        }
      }
    } // pollEvent

    menu.clear(sf::Color::White);
    menu.draw(menu_bg_sprite);
    menu.draw(menu_jouer_sprite);
    menu.draw(menu_quitter_sprite);
    menu.draw(menu_logo_sprite);
    menu.draw(menu_name_sprite);
    menu.display();

  } // menu isOpen



  if (play) {
    
    float ghost_time = GHOST_TIME;

    bool play_with_mouse = true;
    //
    // Game

    // initialize
    std::random_device dev;
    game::Random random(dev());

    b2Vec2 b2_gravity(0.0f, 0.0f);
    b2World b2_world(b2_gravity);

    game::World world;
    game::Element::world = &world;


    world.registerHandler<game::DeadEvent>([&death, &random, &world](game::Entity *entity, game::EventType type, game::Event *event) {
      game::DeadEvent *dead_event = static_cast<game::DeadEvent*>(event);

      auto emitter = new game::ParticleEmitter(random, 300, 0.15);
      emitter->setPosition(game::Distributions::constantDistribution({ dead_event->where.x, dead_event->where.y }));
      emitter->setVelocity(game::Distributions::diskDistribution({ 0.0f, 0.0f }, 30));

      sf::Color color;

      switch (dead_event->what) {
      case game::ElementType::PAPER: // warrior
        color = sf::Color(0x00, 0x80, 0xFF);
        break;
      case game::ElementType::ROCK: // tiger
        color = sf::Color(0x00, 0xFF, 0x00);
        break;
      case game::ElementType::SCISSORS: // mother
        color = sf::Color(0xFF, 0x00, 0xBF);
        break;
      }

      emitter->setColor(game::Distributions::constantDistribution(color));
      emitter->setRadius(game::Distributions::uniformDistribution(0.1f, 0.4f));
      emitter->setPoints(game::Distributions::uniformDistribution(20u, 30u));

      emitter->setLifetime(game::Distributions::uniformDistribution(0.1f, 0.2f));

      auto system = new game::ParticleSystem;
      system->addEmitter(emitter);
      system->addAffector(game::Affectors::fadingAffector(0.2));

      world.addEntity(system, game::Memory::FROM_HEAP);
      
      death.play();

      return game::EventStatus::KEEP;
    });

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), GAME_NAME " (version " GAME_VERSION ")", sf::Style::Titlebar|sf::Style::Close);
    window.setKeyRepeatEnabled(false);

    float ratio = static_cast<float>(SCREEN_HEIGHT) / static_cast<float>(SCREEN_WIDTH);

    sf::View main_view;
    main_view.setCenter(0.0f, 0.0f);
    main_view.setSize(60.0f, 60.0f);
    main_view.setViewport({ 0, 0, ratio, 1.0f });

    sf::View secondary_view({ 0, 0, SCREEN_WIDTH - SCREEN_HEIGHT, SCREEN_HEIGHT });
    secondary_view.setViewport({ ratio, 0, 1.0f - ratio, 1.0f });

    int32 velocity_iterations = 10;
    int32 position_iterations = 8;

    game::WorldListener worldListenerInstance;
    b2_world.SetContactListener(&worldListenerInstance);

    //game::Player player(game::ElementType::ROCK, 0.0f, 0.0f, &b2_world);
    auto player = game::Player::randomGeneration(&b2_world, random);
    world.addEntity(player, game::Memory::FROM_STACK);




    //a static body
    b2BodyDef boundaryDef;
    boundaryDef.type = b2_staticBody;
    boundaryDef.position.Set(0, 0);
    b2Body *staticBody = b2_world.CreateBody(&boundaryDef);

    //shape definition
    b2PolygonShape polygonShape;

    //fixture definition
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polygonShape;
    fixtureDef.filter.categoryBits = game::ElementFunction::BOUNDARY;
    fixtureDef.filter.maskBits = game::ElementFunction::PLAYER | game::ElementFunction::ENEMY | game::ElementFunction::BOUNDARY;

    //add four walls to the static body
    polygonShape.SetAsBox( 31, 1, b2Vec2(0, -30), 0);//ground
    staticBody->CreateFixture(&fixtureDef);
    polygonShape.SetAsBox( 31, 1, b2Vec2(0, 30), 0);//ceiling
    staticBody->CreateFixture(&fixtureDef);
    polygonShape.SetAsBox( 1, 31, b2Vec2(-30, 0), 0);//left wall
    staticBody->CreateFixture(&fixtureDef);
    polygonShape.SetAsBox( 1, 31, b2Vec2(30, 0), 0);//right wall
    staticBody->CreateFixture(&fixtureDef);

    // load resources


    game::Element::warrior=manager.getTexture("warrior.png");
    game::Element::warrior->setSmooth(true);
    game::Element::tiger=manager.getTexture("tiger.png");
    game::Element::tiger->setSmooth(true);
    game::Element::mother=manager.getTexture("mother.png");
    game::Element::mother->setSmooth(true);
    game::Element::bonus=manager.getTexture("bonus.jpeg");
    game::Element::bonus->setSmooth(true);

  #if 0
    sf::Texture *background = manager.getTexture("background.jpg");
    sf::Sprite bgsprite ;
    bgsprite.setScale(0.59,0.79f);
    bgsprite.setPosition(-300,-300);
    bgsprite.setTexture(* background);
  #else
    sf::Texture *background = manager.getTexture("background2.png");
    sf::Sprite bgsprite ;
    bgsprite.setScale(0.1f, 0.1f);
    bgsprite.setPosition(-30, -30);
    bgsprite.setTexture(* background);
  #endif // 0

    sf::Font *font = manager.getFont("segoe.ttf");
    player->getScore()->setFont(font);
    player->getLevel()->setFont(font);

    game::Clock clockElapsed;
    clockElapsed.setFont(font);

    // main loop
    sf::Clock clock;
    while (window.isOpen()) {
      if (ENTITIES_NUMBER + 1 > b2_world.GetBodyCount()) {
        auto elt = game::Element::randomGeneration(&b2_world, random,  player->getElementType(), player->getLevel());
        world.addEntity(elt, game::Memory::FROM_HEAP);
      }

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

            case sf::Keyboard::Space:
              play_with_mouse = !play_with_mouse;
              break;

            default:
              break;
          }
        } else if (event.type == sf::Event::MouseButtonPressed) {
          play_with_mouse = !play_with_mouse;
        }

      }

      // clear when out of screen
      for (b2Body *body = b2_world.GetBodyList(); body != nullptr; body = body->GetNext()) {
        b2Vec2 pos = body->GetPosition();

        if (pos.x < -34 || pos.x > 34 || pos.y < -34 || pos.y > 34) {
          void* bodyUserData = body->GetUserData();

          if (bodyUserData) {
            auto element = static_cast<game::Element *>(bodyUserData);
            element->disappear();
          }
        }

      }

      float vx = 0.0f;
      float vy = 0.0f;

#define SPEED_DIFF 1.0f

      if (play_with_mouse) {
        auto mouse_position = sf::Mouse::getPosition(window);
        auto mouse_world_position = window.mapPixelToCoords(mouse_position, main_view);

        vx = mouse_world_position.x - player->getBody()->GetPosition().x;
        vy = mouse_world_position.y - player->getBody()->GetPosition().y;
      } else {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
          vy -= SPEED_DIFF;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
          vy += SPEED_DIFF;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
          vx -= SPEED_DIFF;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
          vx += SPEED_DIFF;
        }
      }

      float vmax = std::sqrt(vx * vx + vy * vy);

      if (vmax > SPEED_DIFF * 0.5) {
        vx = vx / vmax * PLAYER_SPEED;
        vy = vy / vmax * PLAYER_SPEED;
      } else {
        vx = 0.0f;
        vy = 0.0f;
      }

      player->move(vx, vy);

      // update
      sf::Time elapsed = clock.restart();
      float dt = elapsed.asSeconds();
      b2_world.Step(dt, velocity_iterations, position_iterations);
      world.update(dt);
      clockElapsed.update(dt);

      // render main view
      window.clear(sf::Color::White);
      window.setView(main_view);
      window.draw(bgsprite);
      world.render(window);

      if (player->isGhost() && ghost_time > 0.0f) {
      ghost_time = ghost_time - dt;
      } else {
	ghost_time = GHOST_TIME;
	player->setState(game::ElementState::ALIVE);
      }

    

      sf::RectangleShape rectangle3;
      rectangle3.setSize(sf::Vector2f(0.2,SCREEN_HEIGHT));
      rectangle3.setFillColor(sf::Color(0x10, 0x19, 0x07));
      rectangle3.setPosition(-30, -30);
      window.draw(rectangle3);

      rectangle3.setSize(sf::Vector2f(60,0.2));
      rectangle3.setPosition(-30, -30);
      window.draw(rectangle3);

      rectangle3.setPosition(-30, 29.8);
      window.draw(rectangle3);

      //Render secondary view
      window.setView(secondary_view);
      
      sf::RectangleShape rectangle;
      rectangle.setSize(sf::Vector2f(SCREEN_WIDTH-SCREEN_HEIGHT, SCREEN_HEIGHT));
      rectangle.setFillColor(sf::Color(136, 155, 107));
      rectangle.setPosition(0, 0);
      window.draw(rectangle);

      sf::RectangleShape rectangle2;
      rectangle2.setSize(sf::Vector2f(2, SCREEN_HEIGHT));
      rectangle2.setFillColor(sf::Color(0x10, 0x19, 0x07));
      rectangle2.setPosition(0, 0);
      window.draw(rectangle2);

      sf::RectangleShape rectangle4;
      rectangle4.setSize(sf::Vector2f(2, SCREEN_HEIGHT));
      rectangle4.setFillColor(sf::Color(0x10, 0x19, 0x07));
      rectangle4.setPosition(SCREEN_WIDTH-SCREEN_HEIGHT-2,0);
      window.draw(rectangle4);

      sf::RectangleShape rectangle5;
      rectangle5.setSize(sf::Vector2f(SCREEN_WIDTH-SCREEN_HEIGHT,2 ));
      rectangle5.setFillColor(sf::Color(0x10, 0x19, 0x07));
      rectangle5.setPosition(0,0);
      window.draw(rectangle5);

      rectangle5.setSize(sf::Vector2f(SCREEN_WIDTH-SCREEN_HEIGHT,2 ));
      rectangle5.setFillColor(sf::Color(0x10, 0x19, 0x07));
      rectangle5.setPosition(0,SCREEN_HEIGHT-2);
      window.draw(rectangle5);

      sf::Sprite logo_sprite;
      logo_sprite.setPosition(0, SCREEN_HEIGHT/2+40);
      logo_sprite.setScale(0.30f, 0.30f);
      logo_sprite.setTexture(* menu_logo);
      window.draw(logo_sprite);

      clockElapsed.render(window);
      player->getScore()->render(window);
      player->getLevel()->render(window);

      window.display();
    }
  }
  return 0;
}
