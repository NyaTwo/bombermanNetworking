// input.cpp

#include "input.hpp"

void keyboard::update()
{
   for (auto &key : m_keys) {
      key.m_pressed = false;
      key.m_released = false;
   }
}

bool keyboard::down(const sf::Keyboard::Key key) const
{
   return m_keys[int(key)].m_down;
}

bool keyboard::pressed(const sf::Keyboard::Key key) const
{
   return m_keys[int(key)].m_pressed;
}

bool keyboard::released(const sf::Keyboard::Key key) const
{
   return m_keys[int(key)].m_released;
}
