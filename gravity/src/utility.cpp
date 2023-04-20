// utility.cpp

#include "utility.hpp"

#include <random>

namespace utils
{
   std::random_device                    g_device;
   std::uniform_int_distribution<uint32> g_dist;

   uint32 generate_session_id()
   {
      return g_dist(g_device);
   }

   constexpr timespan kLineLifeTime = timespan::from_seconds(5.0);

   void debug::push(std::string &&text)
   {
      m_lines.emplace_back(std::move(text), kLineLifeTime);
   }

   void debug::update(const float dt)
   {
      // note: remove all old ones
      for (auto it = m_lines.begin(); it != m_lines.end(); ) {
         if ((*it).m_lifetime < timespan::from_seconds(0.0)) {
            it = m_lines.erase(it);
            continue;
         }

         ++it;
      }

      timespan deltatime = timespan::from_seconds(dt);
      for (auto &line : m_lines) {
         line.m_lifetime -= deltatime;
      }
   }
} // !utils

