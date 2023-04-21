// server.hpp

#pragma once

#include <shared.hpp>


struct server {
   struct listener {
      virtual ~listener() = default;
      virtual void on_connect(uint32 id) = 0;
      virtual void on_disconnect(uint32 id, const bool timeout) = 0;
      virtual void on_send(uint32 id, uint32 sequence, byte_stream_writer &writer) = 0;
      virtual void on_receive(uint32 id, uint32 sequence, byte_stream_reader &reader) = 0;
   };

   server(listener &listener);

   bool create(const ip_address &address);
   void destroy();

   void update();
   void receive();
   void transmit();
   
private:

   void perform_timeout_check();
   void remove_disconnected_connections();

   bool contains(const ip_address &address) const;
   void remove_connection(const ip_address &address);
   connection *add_connecting(const ip_address &address, const uint32 session);
   connection *get_connection(const ip_address &address);

   void handle_connect(const ip_address &address, byte_stream_reader &reader);
   void handle_disconnect(const ip_address &address, byte_stream_reader &reader);
   void handle_payload(const ip_address &address, byte_stream_reader &reader);
   void handle_gamestate(const ip_address& adress, byte_stream_reader& reader);

   bool send_connecting(const connection &conn);
   bool send_disconnect(const ip_address &address, uint32 reason);
   bool send_payload(connection &conn);
   bool send_gamestate(connection& conn);
   bool send_byte_stream(const ip_address &address, const byte_stream &stream);
private:
   listener               &m_listener;
   network                 m_network;
   udp_socket              m_socket;
   uint8                   m_currentClientID = 0;
   uint32                  m_connection_counter{ 0 };
   std::vector<connection> m_connections;
   struct {
       float x = 0;
       float y = 0;
   } dynamicPositions[2];
   struct {
       float x = 0;
       float y = 0;
   } staticPositions[2];
};