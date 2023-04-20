// connection.hpp

#pragma once

#include <shared/network.hpp>
#include <shared/timespan.hpp>
#include <shared/utility.hpp>

constexpr timespan kDefaultTimeoutLimit          = timespan::from_seconds(5.0);
constexpr timespan kDefaultSendInterval          = timespan::from_seconds(1.0 / 10.0);
constexpr timespan kDefaultConnectedSendInterval = timespan::from_seconds(1.0 / 20.0);
constexpr timespan kDefaultDisconnectDuration    = timespan::from_seconds(1.0);

struct connection {
   enum class state {
      disconnected,
      connecting,
      connected,
      disconnecting,
   };

   connection();
   connection(const ip_address &address);

   void reset();

   bool is_valid() const;
   bool is_disconnected() const;
   bool is_connected() const;
   bool is_connecting() const;
   bool is_disconnecting() const;
   bool is_timed_out() const;
   bool is_time_to_send() const;
   bool has_address(const ip_address &address) const;
   
   void set_id(uint32 id);
   void set_address(const ip_address &address);
   void set_state(const state next_state);
   void set_session(const uint32 session);
   void set_last_send(const timespan &time);
   void set_last_recieved(const timespan &time);
   void set_timeout_limit(const timespan &time);
   void set_send_interval(const timespan &time);
   void set_acknowledge(const uint32 acknowledge);
   void set_disconnect_reason(const uint32 reason);
   void set_disconnect_time(const timespan &time);
   void set_roundtrip_time(const timespan &time);

   void increment_sequence();
   timespan get_current_roundtrip_time() const;

   ip_address m_address;
   uint32     m_id{ 0 };
   state      m_state{};
   uint32     m_session{ 0 };
   uint32     m_sequence{ 0 };
   uint32     m_acknowledge{ 0 };
   timespan   m_last_receive_time;
   timespan   m_last_send_time;
   timespan   m_timeout_limit;
   timespan   m_send_interval;
   timespan   m_disconnect_time;
   timespan   m_round_trip;
   uint32     m_disconnect_reason{ 0 };
   sequence_buffer<timespan, 256> m_rtt_buffer;
};
