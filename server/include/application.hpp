// application.hpp

#pragma once

#include "server.hpp"

constexpr timespan kTickRateLimit = timespan::from_seconds(1.0 / 60.0);

struct client {
   client() = default;
   client(uint32 id);

   uint32 m_id{ 0 };
   uint32 m_tick{ 0 };
};

struct application final : server::listener {
   application();

   void run();

private:
   bool enter();
   void exit();
   bool tick(const timespan &deltatime);

private: // note: server::listener impl
   void on_connect(uint32 id);
   void on_disconnect(uint32 id, const bool timeout);
   void on_send(uint32 id, uint32 sequence, byte_stream_writer &writer);
   void on_receive(uint32 id, uint32 sequence, byte_stream_reader &reader);

private:
   timespan            m_accumulator;
   uint32              m_tick{ 0 };
   server              m_server;
   std::vector<client> m_clients;
};
