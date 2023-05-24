#pragma once
#include <shared.hpp>
class serverDiscovery {
private:
	ip_address serverIP;
	ip_address broadcastIP;
	udp_socket m_socket;
	float sendTimer = 60;
	float recieveTimer = 6;
	bool foundLocalAddresses = false;
	bool socketOpen = false;
	std::vector<ip_address> addresses;
public:
	serverDiscovery();
	~serverDiscovery();
	bool serverFind(float deltatime);
	ip_address getServerIP();
};