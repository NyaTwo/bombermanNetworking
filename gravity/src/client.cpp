// client.cpp

#include "client.hpp"
//server discovery class
//multiple sockets
//broadcast connect packet
client::client(listener &listener) 
   : m_listener(listener)
{
}

bool client::create()
{
    if (!m_socket.open()) {
        return false;
    }
   return true;
}

void client::destroy()
{
   m_socket.close();
}

void client::connect(const ip_address &address, uint32 session)
{
   if (!m_connection.is_disconnected()) {
      return;
   }
   
   m_connection.set_state(connection::state::connecting);
   m_connection.set_session(session);
   m_connection.set_address(address);
}

void client::client::disconnect()
{
   if (!m_connection.is_connected()) {
      return;
   }

   m_connection.set_state(connection::state::disconnecting);
   m_connection.set_send_interval(kDefaultSendInterval);
   m_connection.set_disconnect_time(timespan::time_since_start());
}

void client::update()
{
   m_inputinator.predictInput(getPosOfDynamicEntities(1));
   perform_timeout_check();
   receive();
}

void client::receive()
{
   
   m_deltatime = m_clock.restart();
   m_apptime += m_deltatime;
   ip_address address;
   byte_stream stream;
   if (m_socket.receive(address, stream)) {
      byte_stream_reader reader(stream);
      auto type = (protocol_packet_type)reader.peek();
      auto messageType = (message_type)reader.peek();
      if (messageType == message_type::gameplay_info_message) {
          m_lastGameState = m_currGameState;
          handle_gamestate(address, reader);
          m_currGameState = m_apptime.asSeconds();
          m_nrOfMessages++;
          m_bytes += 1024;
      }
      else if (type == protocol_packet_type::connect) {
         handle_connect(address, reader);
         m_nrOfPackets++;
         m_bytes += 1024;
      }
      else if (type == protocol_packet_type::disconnect) {
         handle_disconnect(address, reader);
         m_nrOfPackets++;
         m_bytes += 1024;
      }
      else if (type == protocol_packet_type::payload) {
         handle_payload(address, reader);
         m_nrOfPackets++;
         m_bytes += 1024;
      }
      else {
         assert(!"Invalid connection packet received!");
         m_nrOfPackets++;
         m_bytes += 1024;
      }
   }
   else {
      auto error = network::get_last_error();
      if (error.is_critical()) {
         assert(!"Critical network error!");
      }
   }
}

void client::transmit()
{
   if (m_connection.is_disconnected()) {
      return;
   }

   const timespan current_time = timespan::time_since_start();
   if (m_connection.is_time_to_send()) {
      if (m_connection.is_connected()) {
         if (send_payload()) {
            m_connection.set_last_send(current_time);
            m_nrOfPackets++;
            m_bytes += 1024;
         }
         if (send_gamestate()) {
             m_connection.set_last_send(current_time);
             m_nrOfMessages++;
             m_bytes += 1024;
         }
         
      }
      else if (m_connection.is_connecting()) {
         if (send_connecting()) {
            m_connection.set_last_send(current_time);
            m_nrOfPackets++;
            m_bytes += 1024;
         }
      }
      else if (m_connection.is_disconnecting()) {
         const timespan disconnect_duration = current_time - m_connection.m_disconnect_time;
         if (disconnect_duration >= kDefaultDisconnectDuration) {
            m_listener.on_disconnect(false);
            m_connection.reset();
            m_nrOfPackets++;
            m_bytes += 1024;
         }
         else {
            send_disconnect();
            m_connection.set_last_send(current_time);
            m_nrOfPackets++;
            m_bytes += 1024;
         }
      }
   }
}

bool client::is_disconnected() const
{
   return m_connection.is_disconnected();
}

bool client::is_connected() const
{
   return m_connection.is_connected();
}

bool client::is_connecting() const
{
   return m_connection.is_connecting();
}

bool client::is_disconnecting() const
{
   return m_connection.is_disconnecting();
}

timespan client::get_rtt() const
{
   return m_connection.get_current_roundtrip_time();
}

bool client::is_from_server(const ip_address &address) const
{
   return m_connection.m_address == address;
}

void client::perform_timeout_check()
{
   if (m_connection.is_connected() && m_connection.is_timed_out()) {
      m_listener.on_disconnect(true);
      m_connection.reset();
   }
}

void client::handle_connect(const ip_address &address, byte_stream_reader &reader)
{
   if (!is_from_server(address)) {
      return;
   }

   connection_packet packet;
   if (!packet.read(reader)) {
      return;
   }

   if (m_connection.is_connecting()) {
      m_connection.set_last_recieved(timespan::time_since_start());
      if (m_connection.m_session == packet.m_session) {
         m_listener.on_connect();
         m_connection.set_state(connection::state::connected);
         m_connection.set_send_interval(kDefaultConnectedSendInterval);
      }
      else {
         m_listener.on_disconnect(false);
         m_connection.set_state(connection::state::disconnected);
         m_connection.set_send_interval(kDefaultSendInterval);
      }
   }
}

void client::handle_payload(const ip_address &address, byte_stream_reader &reader)
{
   if (!is_from_server(address)) {
      return;
   }

   payload_packet packet;
   if (!packet.read(reader)) {
      return;
   }

   if (m_connection.is_connected()) {
      if (m_connection.m_acknowledge < packet.m_sequence) {
         const timespan process_time(int64(packet.m_process_time_us));
         const timespan send_time = m_connection.m_rtt_buffer.get(packet.m_acknowledge);
         const timespan roundtrip = timespan::time_since_start() - send_time - process_time;
         m_connection.set_roundtrip_time(roundtrip);
         m_connection.set_acknowledge(packet.m_sequence);
         m_connection.set_last_recieved(timespan::time_since_start());
         m_listener.on_receive(packet.m_acknowledge, reader);
      }
   }
}

void client::handle_gamestate(const ip_address& address, byte_stream_reader& reader)
{
    if (!is_from_server(address)) {
        return;
    }

    gameplay_info_message info;
    if (!info.read(reader)) {
        return;
    }

    if (m_connection.is_connected()) {
        if (m_connection.m_acknowledge < info.m_sequence) {
            for (int i = 0; i < 4; i++) {
                if (i == m_clientID) {
                    continue;
                }
                else {
                    sf::Vector2f pos = sf::Vector2f(info.dynamicEntityPositions[i].x, info.dynamicEntityPositions[i].y);
                    if (m_inputinator.validateInput(pos)) {
                        m_entityPositions[i] = pos;
                    }
                    else {
                        m_inputinator.handleMisprediction(pos);
                        m_mispredictions++;
                        m_entityPositions[i] = pos;
                    }
                    
                }
            }
            for (int i = 0; i < 2; i++) {
                m_staticEntityPositions[i].x = info.staticEntityPositions[i].x;
                m_staticEntityPositions[i].y = info.staticEntityPositions[i].y;
            }
            setPlayerPos(sf::Vector2f(m_entityPositions[m_clientID]));
            m_listener.on_receive(info.m_acknowledge, reader);
        }
    }
}

void client::handle_disconnect(const ip_address &address, byte_stream_reader &reader)
{
   if (!is_from_server(address)) {
      return;
   }

   disconnect_packet packet;
   if (!packet.read(reader)) {
      return;
   }

   if (m_connection.is_disconnecting()) {
      m_listener.on_disconnect(false);
      m_connection.reset();
   }
   else if (m_connection.is_connected()) {
      m_connection.set_state(connection::state::disconnecting);
      m_connection.set_send_interval(kDefaultSendInterval);
      m_connection.set_disconnect_reason(packet.m_reason);
      m_connection.set_disconnect_time(timespan::time_since_start());
   }
}

void client::send_disconnect()
{
   byte_stream stream;
   byte_stream_writer writer(stream);
   disconnect_packet packet((disconnect_reason_type)m_connection.m_disconnect_reason);
   if (packet.write(writer)) {
      if (!send_byte_stream(stream)) {
         // note: do we care that if failed?
      }
   }
}

bool client::send_connecting()
{
   byte_stream stream;
   byte_stream_writer writer(stream);
   connection_packet packet(m_connection.m_session);
   if (packet.write(writer)) {
      if (send_byte_stream(stream)) {
         return true;
      }
   }

   return false;
}

bool client::send_payload()
{
   const timespan current_time = timespan::time_since_start();
   m_connection.m_rtt_buffer.set(m_connection.m_sequence, current_time);

   const timespan process_time = current_time - m_connection.m_last_receive_time;
   const uint32 process_time_us = uint32(process_time.elapsed_microseconds());

   byte_stream stream;
   byte_stream_writer writer(stream);
   payload_packet packet(m_connection.m_sequence,
                         m_connection.m_acknowledge,
                         process_time_us);

   if (packet.write(writer)) {
      m_listener.on_send(m_connection.m_sequence, writer);
      m_connection.increment_sequence();
      if (send_byte_stream(stream)) {
         return true;
      }
   }

   return false;
}

bool client::send_gamestate() {
    byte_stream stream;
    byte_stream_writer writer(stream);
    std::vector<float> dynTempArrayX;
    std::vector<float> dynTempArrayY;
    std::vector<float> statTempArrayX;
    std::vector<float> statTempArrayY;
    for (int i = 0; i < 4; i++) {
        dynTempArrayX.push_back(m_entityPositions[i].x);
        dynTempArrayY.push_back(m_entityPositions[i].y);
    }
    for (int i = 0; i < 2; i++) {
        statTempArrayX.push_back(m_staticEntityPositions[i].x);
        statTempArrayY.push_back(m_staticEntityPositions[i].y);
    }
    gameplay_info_message info(m_clientID,m_connection.m_sequence, m_connection.m_acknowledge , dynTempArrayX, dynTempArrayY, statTempArrayX, statTempArrayY);
    if (info.write(writer)) {
        m_listener.on_send(m_connection.m_sequence, writer);
        m_connection.increment_sequence();
        if (send_byte_stream(stream)) {
            return true;
        }
    }
    return false;
}

bool client::send_byte_stream(const byte_stream &stream)
{
   if (!m_socket.send(m_connection.m_address, stream)) {
      auto error = network::get_last_error();
      if (error.is_critical()) {
         assert(!"Critical network error!");
      }

      return false;
   }

   return true;
}

void client::setPlayerPos(sf::Vector2f pos)
{
    m_entityPositions[m_clientID] = pos;
}

void client::setStaticEntityPos(sf::Vector2f pos) {
    m_staticEntityPositions[m_clientID] = pos;
}

void client::resetOtherBombPos() {
    m_staticEntityPositions[1] = sf::Vector2f(0,0);
}

sf::Vector2f client::getPlayerPos()
{
    return getPosOfDynamicEntities(m_clientID);
}

sf::Vector2f client::getPosOfDynamicEntities(int playerNr)
{
    return m_entityPositions[playerNr];
}
sf::Vector2f client::getPosOfStaticEntities(int entityNr)
{
    return m_staticEntityPositions[entityNr];
}

uint8 client::getClientID()
{
    return m_clientID;
}

float client::getTimeBetweenUpdates()
{
    return m_currGameState - m_lastGameState;
}
float client::getTime() {
    float time = 0;
    if (getTimeBetweenUpdates() <= 0) {
        time = 0.75;
    }
    else if (getTimeBetweenUpdates() > 0) {
        time = getTimeBetweenUpdates();
    }
    return time;
}

int client::getNumberOfMessages()
{
    return m_nrOfMessages;
}
int client::getNumberOfPackets()
{
    return m_nrOfPackets;
}
int client::getNumberOfPacketsPerSecond()
{
    return static_cast<int>(m_nrOfPackets / m_apptime.asSeconds());
}
int client::getNumberOfBytesProcessed()
{
    return m_bytes;
}
int client::getNumberOfKiloBytesPerSecond()
{
    return static_cast<int>( (m_bytes/1000) / m_apptime.asSeconds());
}
int client::getNumberOfMispredictions()
{
    return m_mispredictions;
}