// protocol.cpp

#include "shared/protocol.hpp"
#include "shared/network.hpp"

#include <string.h>

connection_packet::connection_packet(const uint32 session,
                                     const uint32 protocol,
                                     const uint32 version)
   : m_type(uint8(protocol_packet_type::connect))
   , m_protocol(protocol)
   , m_version(version)
   , m_session(session)
{
}

bool connection_packet::read(byte_stream_reader &reader)
{
   return serialize(reader);
}

bool connection_packet::write(byte_stream_writer &writer)
{
   return serialize(writer);
}

disconnect_packet::disconnect_packet(const char *reason)
   : m_type(uint8(protocol_packet_type::disconnect))
   , m_reason(uint8(disconnect_reason_type::custom_error_text))
{
   auto len = strlen(reason);
   m_length = len >= sizeof(m_message) ? (sizeof(m_message) - 1) : uint8(len);
   strncpy_s(m_message, reason, m_length);
}

disconnect_packet::disconnect_packet(disconnect_reason_type reason)
   : m_type(uint8(protocol_packet_type::disconnect))
   , m_reason(uint8(reason))
{
}

bool disconnect_packet::read(byte_stream_reader &reader)
{
   return serialize(reader);
}

bool disconnect_packet::write(byte_stream_writer &writer)
{
   return serialize(writer);
}

payload_packet::payload_packet(uint32 sequence,
    uint32 acknowledge,
    uint32 process_time_us)
    : m_type(uint8(protocol_packet_type::payload))
    , m_sequence(sequence)
    , m_acknowledge(acknowledge)
    , m_process_time_us(process_time_us)
{
}

bool payload_packet::read(byte_stream_reader &reader)
{
   return serialize(reader);
}

bool payload_packet::write(byte_stream_writer &writer)
{
   return serialize(writer);
}

