#include "serverDiscovery.h"
#include <iostream>
serverDiscovery::serverDiscovery()
{
}
serverDiscovery::~serverDiscovery()
{
	m_socket.close();
}
bool serverDiscovery::serverFind(float deltaTime)
{
	if (socketOpen == false) {
		if (!m_socket.open()) {
			return false;
		}
		socketOpen = true;
	}
	if (!foundLocalAddresses) {
		if (!ip_address::get_local_addresses(addresses)) {
			return false;
		}
	}
	for (auto&& address : addresses) {
		if (address.a() == 192) {
			broadcastIP = ip_address(address.a(), address.b(), address.c(), 255, 54321);
		}
	}
	sendTimer += deltaTime;
	recieveTimer += deltaTime;
	
	int connectionID = 1;
	connection_packet packet(connectionID);
	const float timeToSendPacket = 6;
	if (sendTimer > timeToSendPacket) {
		byte_stream stream;
		byte_stream_writer writer(stream);
		sendTimer = 0;
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
	
	const float timeToRecievePacket = .6;
	if (recieveTimer > timeToRecievePacket) {
		ip_address tempIp;
		byte_stream stream;
		if (m_socket.receive(tempIp, stream)) {
			byte_stream_reader reader(stream);
			auto type = (protocol_packet_type)reader.peek();
			if (type == protocol_packet_type::connect) {
				serverIP = tempIp;
				recieveTimer = 0;
				return true;
			}
		}
		else {
			auto error = network::get_last_error();
			if (error.is_critical()) {
				assert(!"Critical network error!");
			}
		}
	}
	
	return false;
}

ip_address serverDiscovery::getServerIP()
{
	return serverIP;
}
