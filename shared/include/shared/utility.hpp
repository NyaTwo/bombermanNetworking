// utility.hpp

#pragma once

template <typename T, size_t N>
struct sequence_buffer {
   sequence_buffer() = default;

   void set(uint32 sequence, const T &value) { m_data[sequence % N] = value; }
   T &get(uint32 sequence) { return m_data[sequence % N]; }
   T    get(uint32 sequence) const { return m_data[sequence % N]; }

   T m_data[N];
};
