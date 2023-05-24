// messages.cpp

#include "shared/messages.hpp"

server_info_message::server_info_message()
   : m_type(uint8(message_type::server_info))
{
}

server_info_message::server_info_message(uint32 tick, uint32 clientID)
   : m_type(uint8(message_type::server_info))
   , m_tick(tick)
   , m_clientID(clientID)
{
}

bool server_info_message::read(byte_stream_reader &reader)
{
   return serialize(reader);
}

bool server_info_message::write(byte_stream_writer &writer)
{
   return serialize(writer);
}

gameplay_info_message::gameplay_info_message()
	:m_type(uint8(message_type::gameplay_info_message))
{
}

gameplay_info_message::gameplay_info_message(uint32 clientID, 
	uint32 sequence, 
	uint32 acknowledge, 
	std::vector<float> dynEntX,
	std::vector<float> dynEntY,
	std::vector<float> statEntX,
	std::vector<float> statEntY
	)
	:m_type(uint8(message_type::gameplay_info_message))
	, m_clientID(clientID)
	, m_sequence(sequence)
	, m_acknowledge(acknowledge)
{
	for (int i = 0; i < dynEntX.size(); i++) {
		dynamicEntityPositions[i].x = dynEntX.at(i);
		dynamicEntityPositions[i].y = dynEntY.at(i);
	}
	for (int i = 0; i < statEntX.size(); i++) {
		staticEntityPositions[i].x = statEntX.at(i);
		staticEntityPositions[i].y = statEntY.at(i);
	}
}

bool gameplay_info_message::read(byte_stream_reader& reader)
{
	return serialize(reader);
}

bool gameplay_info_message::write(byte_stream_writer& writer)
{
	return serialize(writer);
}
