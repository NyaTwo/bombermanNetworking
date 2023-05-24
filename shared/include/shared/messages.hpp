// messages.hpp

#pragma once

#include <shared/byte_stream.hpp>
#include <vector>

enum class message_type {
    server_info,
    gameplay_info_message
};

struct server_info_message {
   server_info_message();
   server_info_message(uint32 tick, uint32 clientID);

   bool read(byte_stream_reader &reader);
   bool write(byte_stream_writer &writer);

   template <typename S>
   bool serialize(S &stream)
   {
      if (!stream.serialize(m_type))    { return false; }
      if (!stream.serialize(m_tick))    { return false; }
      if (!stream.serialize(m_clientID)) { return false; }
      return true;
   }

   uint8  m_type{ 0 };
   uint32 m_tick{ 0 };
   uint32 m_clientID{ 0 };
};

struct gameplay_info_message {
    gameplay_info_message();
    gameplay_info_message(uint32 clientID,
        uint32 sequence,
        uint32 acknowledge, 
        std::vector<float> vX, 
        std::vector<float> vY, 
        std::vector<float> statEntX,
        std::vector<float> statEntY
    );
    bool read(byte_stream_reader& reader);
    bool write(byte_stream_writer& writer);

    template <typename S>
    bool serialize(S& stream)
    {
        bool success = true;
        success &= stream.serialize(m_type);
        success &= stream.serialize(m_clientID);
        success &= stream.serialize(m_sequence);
        success &= stream.serialize(m_acknowledge);
        for (auto& pair : dynamicEntityPositions) {
            success &= stream.serialize(pair.x);
            success &= stream.serialize(pair.y);
        }
        for (auto& pair : staticEntityPositions) {
            success &= stream.serialize(pair.x);
            success &= stream.serialize(pair.y);
        }
        return success;
    }

    uint8  m_type{ 0 };
    uint32  m_clientID{ 0 };
    uint32 m_sequence{ 0 };
    uint32 m_acknowledge{ 0 };
    struct {
        float x = 0;
        float y = 0;
    } dynamicEntityPositions[4];
    struct {
        float x = 0;
        float y = 0;
    } staticEntityPositions[2];
};
