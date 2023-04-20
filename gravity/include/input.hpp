// input.hpp

#pragma once

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

struct keyboard final {
   keyboard() = default;

   void update();

   bool down(const sf::Keyboard::Key key) const;
   bool pressed(const sf::Keyboard::Key key) const;
   bool released(const sf::Keyboard::Key key) const;

   struct {
      bool m_down;
      bool m_pressed;
      bool m_released;
   } m_keys[int(sf::Keyboard::KeyCount)]{};
};
