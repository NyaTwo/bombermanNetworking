// timespan.hpp

#pragma once

using int64 = signed long long;

struct timespan {
   static constexpr timespan from_seconds(double value)      { return timespan(int64(value * 1000000.0)); }
   static constexpr timespan from_milliseconds(double value) { return timespan(int64(value * 1000.0)); }

   static timespan time_since_start();

   constexpr timespan() = default;
   constexpr timespan(int64 duration) : m_duration(duration) {}

   constexpr timespan  operator+ (const timespan &rhs) const { return { m_duration + rhs.m_duration }; }
   constexpr timespan  operator- (const timespan &rhs) const { return { m_duration - rhs.m_duration }; }
   constexpr timespan &operator+=(const timespan &rhs)       { m_duration += rhs.m_duration; return *this; }
   constexpr timespan &operator-=(const timespan &rhs)       { m_duration -= rhs.m_duration; return *this; }

   constexpr bool operator==(const timespan &rhs) const { return m_duration == rhs.m_duration; }
   constexpr bool operator!=(const timespan &rhs) const { return m_duration != rhs.m_duration; }
   constexpr bool operator< (const timespan &rhs) const { return m_duration <  rhs.m_duration; }
   constexpr bool operator<=(const timespan &rhs) const { return m_duration <= rhs.m_duration; }
   constexpr bool operator> (const timespan &rhs) const { return m_duration >  rhs.m_duration; }
   constexpr bool operator>=(const timespan &rhs) const { return m_duration >= rhs.m_duration; }

   float elapsed_seconds() const;
   float elapsed_milliseconds() const;
   int64 elapsed_microseconds() const;

   int64 m_duration{ 0 };
};
