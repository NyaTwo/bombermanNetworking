// connection.cpp

#include "shared/connection.hpp"

connection::connection()
   : m_timeout_limit(kDefaultTimeoutLimit)
   , m_send_interval(kDefaultSendInterval)
{
}

connection::connection(const ip_address &address)
   : m_address(address)
   , m_timeout_limit(kDefaultTimeoutLimit)
   , m_send_interval(kDefaultSendInterval)
{
}

void connection::reset()
{
   m_address = ip_address{};
   m_id = 0;
   m_state = state::disconnected;
   m_session = 0;
   m_sequence = 0;
   m_acknowledge = 0;
   m_last_receive_time = timespan{};
   m_last_send_time = timespan{};
   m_timeout_limit = kDefaultTimeoutLimit;
   m_send_interval = kDefaultSendInterval;
   m_disconnect_time = timespan{};
}

bool connection::is_valid() const
{
   return m_address.m_host != ip_address::k_any_host && 
          m_address.m_port != ip_address::k_any_port;
}

bool connection::is_disconnected() const
{
   return m_state == state::disconnected;
}

bool connection::is_connected() const
{
   return m_state == state::connected;
}

bool connection::is_connecting() const
{
   return m_state == state::connecting;
}

bool connection::is_disconnecting() const
{
   return m_state == state::disconnecting;
}

bool connection::is_timed_out() const
{
   if ((m_last_receive_time + m_timeout_limit) < timespan::time_since_start()) {
      return true;
   }

   return false;
}

bool connection::is_time_to_send() const
{
   if ((m_last_send_time + m_send_interval) < timespan::time_since_start()) {
      return true;
   }

   return false;
}

bool connection::has_address(const ip_address &address) const
{
   return m_address == address;
}

void connection::set_address(const ip_address &address)
{
   m_address = address;
}

void connection::set_id(uint32 id)
{
   m_id = id;
}

void connection::set_state(const state s)
{
   m_state = s;
}

void connection::set_session(const uint32 session)
{
   m_session = session;
}

void connection::set_last_send(const timespan &time)
{
   m_last_send_time = time;
}

void connection::set_last_recieved(const timespan &time)
{
   m_last_receive_time = time;
}

void connection::set_timeout_limit(const timespan &time)
{
   m_timeout_limit = time;
}

void connection::set_send_interval(const timespan &time)
{
   m_send_interval = time;
}

void connection::set_acknowledge(const uint32 remote_seq_no)
{
   m_acknowledge = remote_seq_no;
}

void connection::set_disconnect_reason(const uint32 reason)
{
   m_disconnect_reason = reason;
}

void connection::set_disconnect_time(const timespan &time)
{
   m_disconnect_time = time;
}

void connection::set_roundtrip_time(const timespan &time)
{
   m_round_trip = time;
}

void connection::increment_sequence()
{
   m_sequence++;
}

timespan connection::get_current_roundtrip_time() const
{
   return m_round_trip;
}
