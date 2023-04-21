#pragma once
#include <shared.hpp>
class serverDiscovery {
private:
	ip_address serverIP;

public:
	serverDiscovery() = default;
	bool serverFind(ip_address localIP);
	ip_address getServerIP();
};