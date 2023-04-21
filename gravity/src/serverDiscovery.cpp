#include "serverDiscovery.h"

bool serverDiscovery::serverFind(ip_address localIP)
{
	udp_socket m_socket;
	if (!m_socket.open()) {
		return false;
	}

	ip_address broadcastIP(localIP.a(), localIP.b(), localIP.c(), 255, 54321);
	//get local address
	//d needs to be 255
	//port 54321
	//send connect packet to 255
	//recieve something from the server
	byte_stream stream;
	byte_stream_writer writer(stream);
	int connectionID = 1;
	connection_packet packet(connectionID);
	if (packet.write(writer)) {
		if (!m_socket.send(broadcastIP, stream)) {
			auto error = network::get_last_error();
			if (error.is_critical()) {
				assert(!"Critical network error!");
			}
			return false;
		}
	}
	
	if (m_socket.receive(serverIP, stream)) {
		byte_stream_reader reader(stream);

	}
	return false;
}

ip_address serverDiscovery::getServerIP()
{
	return serverIP;
}
