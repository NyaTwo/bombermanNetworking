// application.cpp

//improve interpolation?
#include "application.hpp"
#include "utility.hpp"

static ip_address 
get_same_host_server_address()
{
   
   
   std::vector<ip_address> addresses;
   if (!ip_address::get_local_addresses(addresses)) {
      return {};
   }

   for (auto &addr : addresses) {
      if (addr.a() == 192) {
         return ip_address{ addr.m_host, kDefaultServerPort };
         break;
      }
   }

   return {};
}

application::application()
   : m_client(*this)
{

}

bool application::enter()
{
   if (!m_font.loadFromFile("assets/proggy_tiny.ttf")) {
      return false;
   }
   
   if (!m_client.create()) {
       return false;
   }

   

   enemy enemy;
   enemy.baseShape.setPosition(gameArea.right, gameArea.bottom);
   m_enemies.push_back(enemy);

   if (m_client.getClientID() == 0) {
       m_player.setPlayerPosition(sf::Vector2f(400.f, 120.f));
   }
   else if (m_client.getClientID() == 1) {
       m_player.setPlayerPosition(sf::Vector2f(880.f, 600.f));
   }
  


   m_gameMap.drawMap(m_boxes, m_walls);

   m_server.setFont(m_font);
   m_server.setCharacterSize(32);
   m_server.setPosition({ 8.0f, 2.0f });

   m_usage.setString("(C) Connect-Disconnect");
   m_usage.setFont(m_font);
   m_usage.setCharacterSize(32);
   m_usage.setPosition({ 8.0f, 0.0f });

   m_state.setString("Disconnected");
   m_state.setFillColor(sf::Color::Red);
   m_state.setFont(m_font);
   m_state.setCharacterSize(32);
   m_state.setPosition({ 8.0f, 20.0f });

   m_playerHealth.setString("Health: " + std::to_string(m_player.getHealth()));
   m_playerHealth.setFillColor(sf::Color::Green);
   m_playerHealth.setFont(m_font);
   m_playerHealth.setCharacterSize(32);
   m_playerHealth.setPosition({ 60.0f, 60.0f });

   m_enemyHealth.setString("Health: " + std::to_string(0));
   m_enemyHealth.setFillColor(sf::Color::Red);
   m_enemyHealth.setFont(m_font);
   m_enemyHealth.setCharacterSize(32);
   m_enemyHealth.setPosition({ gameArea.right + 60.0f, 60.0f });

   m_gameEnd.setFont(m_font);
   m_gameEnd.setCharacterSize(128);
   m_gameEnd.setPosition({ gameArea.right / 2, gameArea.bottom / 2 });

   //m_debug.setString("Disconnected");
   m_debug.setFont(m_font);
   m_debug.setCharacterSize(16);
   
   return true;
}

void application::exit()
{
   m_client.disconnect();
}

bool application::update()
{
    m_deltatime = m_clock.restart();
    m_apptime += m_deltatime;
    if (m_serverDisc.serverFind(m_deltatime.asSeconds()) && !m_serverFound) {
        m_serverIP = m_serverDisc.getServerIP();
        m_serverFound = true;
    }
   
   if (!m_gameEnded) {
       wrapPlayerAroundGameArea();
       if (m_client.is_connected()) {
           gameResult();
           updateEntities(m_deltatime.asSeconds());
           m_tick++;
       }
       m_client.update();
       if (!(m_client.getPosOfStaticEntities(0).x >= gameArea.left) || !(m_client.getPosOfStaticEntities(0).x <= gameArea.right)) {
           m_placedEnemyBomb = false;
       }
       if (m_keyboard.pressed(sf::Keyboard::Key::C)) {
           connect();
           disconnect();
       }
       m_playerHealth.setString("Health: " + std::to_string(m_player.getHealth()));
       m_enemyHealth.setString("Health: " + std::to_string(m_enemies.front().getHealth()));
   }
   { // debug text
      auto text = "Tick: " + std::to_string(m_tick) + " (" + std::to_string(m_server_tick) + ") ";
      text += "RTT:  " + std::to_string(m_client.get_rtt().elapsed_milliseconds()) + "ms ";
      text += "Messages: " + std::to_string(m_client.getNumberOfMessages());
      text += " Packets: " + std::to_string(m_client.getNumberOfPackets());
      text += " Bytes: " + std::to_string(m_client.getNumberOfBytesProcessed());
      text += " Input Commands Mispredictions: " + std::to_string(m_client.getNumberOfMispredictions());
      text += " Kilobytes: " + std::to_string(m_client.getNumberOfKiloBytesPerSecond()) + "/s ";
      text += "Packets Per Second: " + std::to_string(m_client.getNumberOfPacketsPerSecond()) + "/s ";
      m_debug.setString(text);
      auto size = m_window.getSize();
      auto rect = m_debug.getLocalBounds();
      m_debug.setPosition({ 2.0f, size.y - rect.height - m_font.getLineSpacing(16) });
   }
   m_client.transmit();
   m_keyboard.update();
   return m_running;
}

void application::gameResult()
{
    if (m_player.getHealth() == 0 && m_enemies.front().getHealth() == 0) {
        m_gameEnd.setString("Draw!");
        m_gameEnd.setFillColor(sf::Color::Black);
        m_gameEnded = true;
    }
    else if (m_player.getHealth() == 0) {
        m_gameEnd.setString("You Lost!");
        m_gameEnd.setFillColor(sf::Color::Red);
        m_gameEnded = true;
    }
    else if (m_enemies.front().getHealth() == 0) {
        m_gameEnd.setString("You Won!");
        m_gameEnd.setFillColor(sf::Color::Green);
        m_gameEnded = true;
    }
}

void application::disconnect()
{
    if (m_client.is_connected()) {
        m_state.setFillColor(sf::Color::Cyan);
        m_state.setString("Disconnecting...");
        m_server.setString("");
        m_client.disconnect();
        m_tick = 0;
    }
}

void application::connect()
{
    if (m_client.is_disconnected()) {
        m_state.setFillColor(sf::Color::Cyan);
        m_state.setString("Connecting...");

        ip_address address = m_serverIP;
        m_client.connect(address, 1);

        m_server.setString(address.as_string());
        auto size = m_window.getSize();
        auto rect = m_server.getLocalBounds();
        m_server.setPosition(size.x - rect.width - 10.0f, 2.0f);
    }
}

void application::updateEntities(float dt)
{
    m_player.update(dt, m_client, m_keyboard, m_bombs);
    enemyUpdate();
    for (auto& b : m_bombs) {
        b.update(m_deltatime.asSeconds());
        if (b.getHasExploded() == true) {
            m_explosions.push_back(b.getExplosion());
            m_player.bombExploded();
            m_bombs.erase(m_bombs.begin());
        }
    }
    for (auto& e : m_explosions) {
        e.update(m_deltatime.asSeconds());
        if (e.getIsOver()) {
            m_explosions.erase(m_explosions.begin());
        }
    }
    for (auto& en : m_enemies) {
        en.update(dt, m_client);
    }
    checkCollision();
    m_boxes.erase(std::remove_if(m_boxes.begin(), m_boxes.end(), [](box& b) { return b.isDestroyed(); }), m_boxes.end());
}

void application::checkCollision()
{
    for (auto& e : m_explosions) {
        if (e.collisionWithExplosion(m_player)) {
            m_player.hurt();
        }
        if (e.collisionWithExplosion(m_enemies.front())) {
            m_enemies.front().hurt();
        }
        for (auto& bx : m_boxes) {
            if (e.collisionWithExplosion(bx)) {
                bx.destroy();
            }
        }
    }
    for (auto& bx : m_boxes) {
        if (bx.collisionCheck(m_player)) {
            m_player.collidingWithBoxOrWall(m_keyboard);
        }
    }
    for (auto& w : m_walls) {
        if (w.collisionCheck(m_player)) {
            m_player.collidingWithBoxOrWall(m_keyboard);
        }
    }
}

void application::enemyUpdate() {

    if (m_client.getPosOfStaticEntities(1).x >= gameArea.left && m_client.getPosOfStaticEntities(1).x <= gameArea.right && m_placedEnemyBomb == false) {
        m_bombs.push_back(Bomb(m_client.getPosOfStaticEntities(1)));
        m_client.resetOtherBombPos();
        m_placedEnemyBomb = true;
    }
}

void application::render()
{
   m_window.clear(sf::Color{ 0x55,0x55,0x55,0xff });
   if (m_client.is_connected() && !m_gameEnded) {
       for (auto&& e : m_enemies) {
           e.render(m_window);
       }
       m_player.render(m_window);
       for (auto&& b : m_bombs) {
           b.render(m_window);
       }
       for (auto&& bx : m_boxes) {
           bx.render(m_window);
       }
       for (auto&& e : m_explosions) {
           e.render(m_window);
       }
       for (auto&& w : m_walls) {
           w.render(m_window);
       }
       m_window.draw(m_playerHealth);
       m_window.draw(m_enemyHealth);
       m_gameMap.render(m_window);
   }
   m_window.draw(m_usage);
   m_window.draw(m_state);
   m_window.draw(m_server);
   m_window.draw(m_debug);
   m_window.draw(m_gameEnd);
   m_window.display();
}

void application::on_closing()
{
   m_running = false;
}

void application::on_mouse_moved(int x, int y)
{
}

void application::on_key_pressed(sf::Keyboard::Key key)
{
   if (key == sf::Keyboard::Key::Escape) {
      m_running = false;
   }

   m_keyboard.m_keys[int(key)].m_down = true;
   m_keyboard.m_keys[int(key)].m_pressed = true;
   m_keyboard.m_keys[int(key)].m_released = false;
}

void application::on_key_released(sf::Keyboard::Key key)
{
   m_keyboard.m_keys[int(key)].m_down = false;
   m_keyboard.m_keys[int(key)].m_pressed = false;
   m_keyboard.m_keys[int(key)].m_released = true;
}

void application::on_button_pressed(sf::Mouse::Button button)
{
}

void application::on_button_released(sf::Mouse::Button button)
{
}

void application::wrapPlayerAroundGameArea()
{
    sf::Vector2f tempCurrPos = m_player.getPlayerPosition();
    if (tempCurrPos.x < gameArea.left) {
        m_player.setPlayerPosition(sf::Vector2f(gameArea.left, tempCurrPos.y));
    }
    if (tempCurrPos.x > gameArea.right) {
        m_player.setPlayerPosition(sf::Vector2f(gameArea.right, tempCurrPos.y));
    }
    if (tempCurrPos.y < gameArea.top) {
        m_player.setPlayerPosition(sf::Vector2f(tempCurrPos.x, gameArea.top));
    }
    if (tempCurrPos.y > gameArea.bottom) {
        m_player.setPlayerPosition(sf::Vector2f(tempCurrPos.x, gameArea.bottom));
    }
}

void application::on_connect()
{
   m_state.setFillColor(sf::Color::Green);
   m_state.setString("Connected!");
}

void application::on_disconnect(const bool timeout)
{
   m_state.setFillColor(sf::Color::Red);
   m_state.setString(timeout ? "Timedout" : "Disconnected");
   m_server.setString("");

   m_tick = 0;
}

void application::on_send(uint32 sequence, byte_stream_writer &writer)
{
   server_info_message message(m_tick);
   if (!message.write(writer)) {
      assert(!"could not write server_info_message!");
   }

}

void application::on_receive(uint32 sequence, byte_stream_reader &reader)
{
   while (reader.has_data()) {
      auto type = (message_type)reader.peek();
      if (type == message_type::server_info) {
         server_info_message message;
         if (!message.read(reader)) {
            assert(!"could not read server_info_message!");
         }
         m_server_tick = message.m_tick;
      }
      else {
         assert(!"unknown message type!");
      }
   }
}
