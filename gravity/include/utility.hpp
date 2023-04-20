// utility.hpp

#pragma once

#include <vector>
#include <string>
#include <shared/timespan.hpp>

using uint32 = unsigned int;

namespace utils
{
   uint32 generate_session_id();

   struct debug {
      debug() = default;

      void push(std::string &&text);
      void update(const float deltatime);

      struct entry {
         std::string m_text;
         timespan    m_lifetime;
      };
      std::vector<entry> m_lines;
   };
} // !utils
