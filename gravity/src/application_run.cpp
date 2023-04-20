// appliction_run.cpp

#include "application.hpp"

#include <SFML/Graphics.hpp>

void application::run() 
{
   m_window.create(sf::VideoMode(1280, 720), "gravity", sf::Style::Close | sf::Style::Titlebar);
   m_window.setKeyRepeatEnabled(false);
   m_window.setVerticalSyncEnabled(true);

   if (!enter()) {
      return;
   }

   while (m_window.isOpen()) {
      sf::Event event;
      while (m_window.pollEvent(event)) {
         if (event.type == sf::Event::MouseMoved) {
            on_mouse_moved(event.mouseMove.x, event.mouseMove.y);
         }
         else if (event.type == sf::Event::KeyPressed) {
            on_key_pressed(event.key.code);
         }
         else if (event.type == sf::Event::KeyReleased) {
            on_key_released(event.key.code);
         }
         else if (event.type == sf::Event::MouseButtonPressed) {
            on_button_pressed(event.mouseButton.button);
         }
         else if (event.type == sf::Event::MouseButtonReleased) {
            on_button_released(event.mouseButton.button);
         }
         else if (event.type == sf::Event::Closed) {
            on_closing();
            m_window.close();
         }
      }

      if (!update()) {
         m_window.close();
      }

      render();
   }

   exit();
}

keyboard application::getKeyboard()
{
    return m_keyboard;
}
