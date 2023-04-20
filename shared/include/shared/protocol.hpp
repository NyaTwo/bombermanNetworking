// protocol.hpp

#pragma once

#include <shared/byte_stream.hpp>

static constexpr uint32 kProtocolIdent     = 0x11223344u;
static constexpr uint32 kProtocolVersion   = 0x00010000u;
static constexpr uint16 kDefaultServerPort = 54321;

// note: simple connection protocol packet types
enum class protocol_packet_type 
{
   connect,
   disconnect,
   payload,
   count,
};

// note: some basic disconnect reason types
enum class disconnect_reason_type 
{
   custom_error_text,
   wrong_ident,
   wrong_version,
   server_full,
   server_closing,
   connection_timedout,
   // ...
   unknown_error,
   count,
};

struct connection_packet {
   connection_packet() = default;
   connection_packet(const uint32 session,
                     const uint32 protocol = kProtocolIdent,
                     const uint32 version = kProtocolVersion);

   bool read(byte_stream_reader &reader);
   bool write(byte_stream_writer &writer);

   template <typename S>
   bool serialize(S &stream)
   {
      bool success = true;
      success &= stream.serialize(m_type);
      success &= stream.serialize(m_protocol);
      success &= stream.serialize(m_version);
      success &= stream.serialize(m_session);
      return success;
   }

   uint8  m_type{ 0 };
   uint32 m_protocol{ 0 };
   uint32 m_version{ 0 };
   uint32 m_session{ 0 };
};

struct disconnect_packet {
   disconnect_packet() = default;
   disconnect_packet(const char *reason);
   disconnect_packet(disconnect_reason_type reason);

   bool read(byte_stream_reader &reader);
   bool write(byte_stream_writer &writer);

   template <typename S>
   bool serialize(S &stream)
   {
      bool success = true;
      success &= stream.serialize(m_type);
      success &= stream.serialize(m_reason);
      return success;
   }

   uint8 m_type{ 0 };
   uint8 m_reason{ 0 };
   uint8 m_length{ 0 };
   char  m_message[256]{};
};

struct payload_packet {
   payload_packet() = default;
   payload_packet(uint32 sequence,
                  uint32 acknowledge,
                  uint32 process_time_us);

   bool read(byte_stream_reader &reader);
   bool write(byte_stream_writer &writer);

   template <typename S>
   bool serialize(S &stream)
   {
      bool success = true;
      success &= stream.serialize(m_type);
      success &= stream.serialize(m_sequence);
      success &= stream.serialize(m_acknowledge);
      success &= stream.serialize(m_process_time_us);
      return success;
   }

   uint8  m_type{ 0 };
   uint32 m_sequence{ 0 };
   uint32 m_acknowledge{ 0 };
   uint32 m_process_time_us{ 0 };
};
