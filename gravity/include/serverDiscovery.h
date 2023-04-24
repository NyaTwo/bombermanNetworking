#pragma once
#include <shared.hpp>
class serverDiscovery {
private:
	ip_address serverIP;
	ip_address broadcastIP;
	udp_socket m_socket;
	float timer = 6;
public:
	serverDiscovery() = default;
	~serverDiscovery();
	bool serverFind(float deltatime);
	ip_address getServerIP();
};