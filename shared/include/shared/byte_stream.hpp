// byte_stream.hpp

#pragma once

#include <type_traits> // std::is_fundamental
#include <cstring>     // std::memcpy
#include <cassert>

using uint64 = unsigned long long;
using  int64 =   signed long long;
using uint32 = unsigned int;
using  int32 =   signed int;
using uint16 = unsigned short;
using  int16 =   signed short;
using  uint8 = unsigned char;
using   int8 =   signed char;

struct byte_stream {
   byte_stream() = default;

   int  m_size{ 0 };
   char m_buffer[1024] = {};
};

struct byte_stream_writer {
   byte_stream_writer(byte_stream &stream)
      : m_stream(stream)
      , m_cursor(stream.m_buffer + stream.m_size)
      , m_end(stream.m_buffer + sizeof(stream.m_buffer))
   {
   }

   template <typename T>
   bool serialize(T value) {
      static_assert(std::is_fundamental_v<T>, "T needs to be a fundamental datatype!");
      if ((m_cursor + sizeof(T)) >= m_end) {
         return false;
      }

      std::memcpy(m_cursor, &value, sizeof(T));
      m_cursor += sizeof(T);
      m_stream.m_size = int32_t(m_cursor - m_stream.m_buffer);
      return true;
   }

   byte_stream &m_stream;
   char        *m_cursor{ nullptr };
   char        *m_end{ nullptr };
};

struct byte_stream_reader {
   byte_stream_reader(byte_stream &stream)
      : m_stream(stream)
      , m_cursor(stream.m_buffer)
      , m_end(stream.m_buffer + stream.m_size)
   {
   }

   bool has_data() const
   {
      return m_cursor < m_end;
   }

   uint8 peek() const
   {
      return m_cursor[0];
   }

   template <typename T>
   bool serialize(T &value) {
      static_assert(std::is_fundamental_v<T>, "T needs to be a fundamental datatype!");
      if ((m_cursor + sizeof(T)) > m_end) {
         return false;
      }

      std::memcpy(&value, m_cursor, sizeof(T));
      m_cursor += sizeof(T);
      return true;
   }

   byte_stream &m_stream;
   char        *m_cursor{ nullptr };
   char        *m_end{ nullptr };
};
