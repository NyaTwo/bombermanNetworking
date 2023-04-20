// timespan.cpp

#include "shared/timespan.hpp"

#include <chrono>

// static 
timespan timespan::time_since_start()
{
   static int64 start = 0;
   if (start == 0) {
      auto ns = std::chrono::high_resolution_clock::now();
      auto ms = std::chrono::time_point_cast<std::chrono::microseconds>(ns);
      start = ms.time_since_epoch().count();
   }

   auto ns = std::chrono::high_resolution_clock::now();
   auto ms = std::chrono::time_point_cast<std::chrono::microseconds>(ns);
   auto now = ms.time_since_epoch().count();

   return timespan{ now - start };
}

float timespan::elapsed_seconds() const
{
   return float(m_duration / 1000000.0);
}

float timespan::elapsed_milliseconds() const
{
   return float(m_duration / 1000.0);
}

int64 timespan::elapsed_microseconds() const
{
   return m_duration;
}
