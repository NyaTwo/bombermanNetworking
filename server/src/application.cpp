// application.cpp

#include "application.hpp"
#include <stdio.h>

client::client(uint32 id) 
   : m_id(id)
{
}

application::application()
   : m_server(*this)
{
}

void application::run()
{
   if (!enter()) {
      return;
   }

   timespan previous = timespan::time_since_start();
   timespan deltatime;
   while (tick(deltatime)) {
      timespan current = timespan::time_since_start();
      deltatime = current - previous;
      previous = current;
   }

   exit();
}

bool application::enter()
{
   if (!m_server.create(ip_address{ ip_address::k_any_host, kDefaultServerPort })) {
      auto error = network::get_last_error();
      printf("err: could not create server (%d - %s)\n", error.as_code(), error.as_string());
      return false;
   }

   printf("server running...\n");

   return true;
}

void application::exit()
{
}

bool application::tick(const timespan &deltatime)
{
   m_server.update();

   m_accumulator = m_accumulator + deltatime;
   while (m_accumulator >= kTickRateLimit) {
      m_accumulator = m_accumulator - kTickRateLimit;

      // todo: gameplay logic

      m_tick++;
      m_server.transmit();
   }

   return true;
}

void application::on_connect(uint32 id)
{
   printf("nfo: %d connected\n", id);
   m_clients.emplace_back(id);
}

void application::on_disconnect(uint32 id, const bool timeout)
{
   for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
      if ((*it).m_id == id) {
         printf("nfo: %d %s\n", id, timeout ? "timedout" : "disconnected");
         m_clients.erase(it);
         break;
      }
   }
}

void application::on_send(uint32 id, uint32 sequence, byte_stream_writer &writer)
{
   for (auto &p : m_clients) {
      if (p.m_id == id) {
         server_info_message message(m_tick);
         if (!message.write(writer)) {
            printf("err: failed to write server_info_message!\n");
         }

         break;
      }
   }
}

void application::on_receive(uint32 id, uint32 sequence, byte_stream_reader &reader)
{
   for (auto &p : m_clients) {
      if (p.m_id == id) {
         while (reader.has_data()) {
            auto type = (message_type)reader.peek();
            if (type == message_type::server_info) {
               server_info_message message;
               if (!message.read(reader)) {
                  printf("err: failed to read server_info_message!\n");
                  break;
               }

               p.m_tick = message.m_tick;
            }
            else {
               assert(!"unknown message type!");
            }
         }

         break;
      }
   }
}
