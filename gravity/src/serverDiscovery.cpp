#include "serverDiscovery.h"
serverDiscovery::~serverDiscovery()
{
	m_socket.close();
}
bool serverDiscovery::serverFind(float deltaTime)
{
	
	if (!m_socket.open()) {
		return false;
	}
	
	std::vector<ip_address> addresses; 
	if (!ip_address::get_local_addresses(addresses)) {
		return false;
	}
	
	for (auto&& address : addresses) {
		if (address.a() == 192) {
			broadcastIP = ip_address(address.a(), address.b(), address.c(), 255, 54321);
		}
	}
	timer += deltaTime;
	byte_stream stream;
	byte_stream_writer writer(stream);
	int connectionID = 1;
	connection_packet packet(connectionID);
	const float timeToSendPacket = 20;
	if (timer > timeToSendPacket) {
		timer = 0;
		if (packet.write(writer)) {
			if (!m_socket.send(broadcastIP, stream)) {
				auto error = network::get_last_error();
				if (error.is_critical()) {
					assert(!"Critical network error!");
				}
				return false;
			}
		}
	}
	if (m_socket.receive(serverIP, stream)) {
		byte_stream_reader reader(stream);
		return true;
	}
	else {
		auto error = network::get_last_error();
		if (error.is_critical()) {
			assert(!"Critical network error!");
		}
	}
	return false;
}

ip_address serverDiscovery::getServerIP()
{
	return serverIP;
}
