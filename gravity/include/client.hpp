// client.hpp

#pragma once
#include <SFML/Graphics.hpp>
#include <shared.hpp>
#include <interpolator.h>
#include <inputinator.h>

struct client {
   struct listener {
      virtual ~listener() = default;
      virtual void on_connect() = 0;
      virtual void on_disconnect(const bool timeout) = 0;
      virtual void on_send(uint32 sequence, byte_stream_writer &writer) = 0;
      virtual void on_receive(uint32 sequence, byte_stream_reader &reader) = 0;
   };
   client(listener &listener);

   bool create();
   void destroy();

   void connect(const ip_address &address, uint32 session);
   void disconnect();

   void update();
   void transmit();

   bool is_disconnected() const;
   bool is_connected() const;
   bool is_connecting() const;
   bool is_disconnecting() const;

   timespan get_rtt() const;

   void setPlayerPos(sf::Vector2f pos);
   void setStaticEntityPos(sf::Vector2f pos);
   void resetOtherBombPos();

   sf::Vector2f getPlayerPos();
   sf::Vector2f getPosOfDynamicEntities(int enemyNr);
   sf::Vector2f getPosOfStaticEntities(int entityNr);
   uint8        getClientID();

   float        getTime();
   int          getNumberOfMessages();
   int          getNumberOfPackets();
   int          getNumberOfPacketsPerSecond();
   int          getNumberOfBytesProcessed();
   int          getNumberOfKiloBytesPerSecond();
   int          getNumberOfMispredictions();

private:
   void receive();
   bool is_from_server(const ip_address &address) const;
   void perform_timeout_check();

   void handle_connect(const ip_address &address, byte_stream_reader &reader);
   void handle_payload(const ip_address &address, byte_stream_reader &reader);
   void handle_gamestate(const ip_address& address, byte_stream_reader& reader);
   void handle_disconnect(const ip_address &address, byte_stream_reader &reader);

   void send_disconnect();
   bool send_connecting();
   bool send_payload();
   bool send_gamestate();
   bool send_byte_stream(const byte_stream &stream);

   float        getTimeBetweenUpdates();

private:
   sf::Time     m_deltatime;
   sf::Time     m_apptime;
   Interpolator m_interpolator;
   inputinator  m_inputinator;
   listener     &m_listener;
   network      m_network;
   udp_socket   m_socket;
   connection   m_connection;
   sf::Clock    m_clock;
   sf::Vector2f m_entityPositions[4];
   sf::Vector2f m_staticEntityPositions[2];
   uint8        m_clientID = 0;
   uint32       m_clientTick;
   
private:
    float        m_lastGameState = 0;
    float        m_currGameState = 0;
    int          m_nrOfMessages = 0;
    int          m_nrOfPackets = 0;
    int          m_bytes = 0;
    int          m_mispredictions = 0;
};
