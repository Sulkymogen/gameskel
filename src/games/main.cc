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

#include <Box2D/Box2D.h>

#include "config.h"

int main() {
  // initialize
  game::Random& m_random ();
  game::Distribution<unsigned> m_axis;// = uniformDistribution(0, 4);
  game::Distribution<float> m_value;// = uniformDistribution(-330.0f, 330.0f);
  game::Distribution<float> m_cible;// = uniformDistribution(-320.0f, 320.0f);
  
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

  // load resources


  // add entities
  game::Element elt(game::ElementType::PAPER, 0.0f, 0.0f, 50.0f, 0.0f, &b2_world);
  world.addEntity(&elt, game::Memory::FROM_STACK);

  game::Element elt2(game::ElementType::ROCK, 100.0f, 0.0f, -100.0f, 0.0f, &b2_world);
  world.addEntity(&elt2, game::Memory::FROM_STACK);

  game::Element elt3(game::ElementType::SCISSORS, -100.0f, 0.0f, 200.0f, 0.0f, &b2_world);
  world.addEntity(&elt3, game::Memory::FROM_STACK);

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
    world.render(window);
    window.display();
  }

  return 0;
}
