// application.hpp

#pragma once

#include <SFML/Graphics.hpp>

#include "input.hpp"
#include "client.hpp"
#include "bomb.h"
#include "player.h"
#include "gameMap.h"
#include "box.h"
#include "wall.h"
#include "enemy.h"
#include <serverDiscovery.h>

class application final : client::listener {
public:
   application();

   void run();
   keyboard getKeyboard();

private:
   bool enter();
   void exit();
   bool update();
   void gameResult();
   void disconnect();
   void connect();
   void updateEntities(float dt);
   void checkCollision();
   void enemyUpdate();
   void render();

private:
   void on_closing();
   void on_mouse_moved(int x, int y);
   void on_key_pressed(sf::Keyboard::Key key);
   void on_key_released(sf::Keyboard::Key key);
   void on_button_pressed(sf::Mouse::Button button);
   void on_button_released(sf::Mouse::Button button);
   void wrapPlayerAroundGameArea();

private: // client::listener impl
   void on_connect();
   void on_disconnect(const bool timeout);
   void on_send(uint32 sequence, byte_stream_writer &writer);
   void on_receive(uint32 sequence, byte_stream_reader &reader);

private:
   sf::RenderWindow		m_window;
   sf::Clock			m_clock;
   sf::Time				m_apptime;
   sf::Time				m_deltatime;
   sf::Font				m_font;
   std::vector<enemy>	m_enemies;
   std::vector<Bomb>	m_bombs;
   std::vector<Explosion> m_explosions;
   std::vector<box>		m_boxes;
   std::vector<wall>	m_walls;
   Interpolator			m_interpolator;
   gameMap				m_gameMap;
   Player				m_player;
   serverDiscovery m_serverDisc;
   ip_address       m_serverIP;
private:
   bool             m_running{ true };
   bool				m_placedEnemyBomb = false;
   bool				m_gameEnded = false;
   bool				m_serverFound = false;
   float			m_timer = 0;
   float			m_timeToServerSearch = 0.6;
   keyboard         m_keyboard;
   client           m_client;
   uint32           m_tick{ 0 };
   uint32           m_server_tick{ 0 };
   sf::Text         m_server;
   sf::Text         m_usage;
   sf::Text         m_state;
   sf::Text         m_debug;
   sf::Text			m_playerHealth;
   sf::Text			m_enemyHealth;
   sf::Text			m_gameEnd;
};
