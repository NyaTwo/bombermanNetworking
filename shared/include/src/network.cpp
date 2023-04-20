// network.cpp

#include "shared/network.hpp"
#include "shared/byte_stream.hpp"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>

network::error::error(int code)
   : m_code(code)
{
}

network::error::error(const error &rhs)
   : m_code(rhs.m_code)
{
}

bool network::error::is_critical() const
{
   if (m_code == 0 || m_code == WSAEWOULDBLOCK || m_code == WSAECONNRESET) {
      return false;
   }

   return true;
}

int32 network::error::as_code() const
{
   return m_code;
}

const char *network::error::as_string() const
{
   switch (m_code) {
      case WSAEADDRINUSE:
         return "Address already in use.";
      case WSAECONNRESET:
         return "Connection reset by peer.";
      case WSAEWOULDBLOCK:
         return "Resource temporarily unavailable.";
      case WSANOTINITIALISED:
         return "Successful WSAStartup not yet performed.";
   }

   return "Unknown socket error!";
}

//static 
network::error network::get_last_error()
{
   return network::error{ WSAGetLastError() };
}

network::network()
{
   WSADATA data{};
   if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
      return;
   }
}

network::~network()
{
   WSACleanup();
}

// static 
bool ip_address::get_local_addresses(std::vector<ip_address> &addresses)
{
   DWORD size = 0;
   GetAdaptersAddresses(AF_INET,
                        GAA_FLAG_INCLUDE_PREFIX,
                        NULL,
                        NULL,
                        &size);

   IP_ADAPTER_ADDRESSES *adapter_addresses = (IP_ADAPTER_ADDRESSES *)calloc(1, size);
   GetAdaptersAddresses(AF_INET,
                        GAA_FLAG_INCLUDE_PREFIX,
                        NULL,
                        adapter_addresses,
                        &size);

   for (IP_ADAPTER_ADDRESSES *iter = adapter_addresses; iter != NULL; iter = iter->Next) {
      if (iter->OperStatus == IfOperStatusUp && (iter->IfType == IF_TYPE_ETHERNET_CSMACD ||
                                                 iter->IfType == IF_TYPE_IEEE80211))
      {
         for (IP_ADAPTER_UNICAST_ADDRESS *ua = iter->FirstUnicastAddress; ua != NULL; ua = ua->Next) {
            char addrstr[1024] = {};
            getnameinfo(ua->Address.lpSockaddr, ua->Address.iSockaddrLength, addrstr, sizeof(addrstr), NULL, 0, NI_NUMERICHOST);

            if (ua->Address.lpSockaddr->sa_family == AF_INET) {
               sockaddr_in ai = *(sockaddr_in *)ua->Address.lpSockaddr;
               ip_address address;
               address.m_host = ntohl(ai.sin_addr.s_addr);
               address.m_port = ntohs(ai.sin_port);
               addresses.push_back(address);
            }
         }
      }
   }

   free(adapter_addresses);

   return !addresses.empty();
}

static uint32 dns_query(const char *name)
{
   addrinfo hints{};
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_protocol = IPPROTO_TCP;

   addrinfo *result{ nullptr };
   int res = getaddrinfo(name, nullptr, &hints, &result);
   if (res == WSAHOST_NOT_FOUND) {
      return 0;
   }

   uint32 host = 0;
   for (addrinfo *it = result; it != nullptr; it = it->ai_next) {
      if (it->ai_family == AF_INET) {
         sockaddr_in &addr = *(sockaddr_in *)it->ai_addr;
         host = ntohl(addr.sin_addr.s_addr);
         break;
      }
   }

   freeaddrinfo(result);

   return host;
}

ip_address::ip_address(const ip_address &rhs)
   : m_host(rhs.m_host)
   , m_port(rhs.m_port)
{
}

ip_address::ip_address(const char *name, uint16 port)
   : m_host(dns_query(name))
   , m_port(port)
{
}

ip_address::ip_address(const uint32 host, uint16 port)
   : m_host(host)
   , m_port(port)
{
}

ip_address::ip_address(uint8 a, uint8 b, uint8 c, uint8 d, uint16 port)
   : m_host(k_any_host)
   , m_port(port)
{
   m_host = uint32(a) << 24;
   m_host |= uint32(b) << 16;
   m_host |= uint32(c) << 8;
   m_host |= uint32(d);
}

bool ip_address::operator==(const ip_address &rhs) const
{
   return m_host == rhs.m_host && m_port == rhs.m_port;
}

bool ip_address::operator!=(const ip_address &rhs) const
{
   return m_host != rhs.m_host || m_port != rhs.m_port;
}

bool ip_address::valid() const
{
   return m_host != k_any_host && m_port != k_any_port;
}

const char *ip_address::as_string() const
{
   // danger: threadsafe this is not!
   static char str[64]{};
   sprintf_s(str, "%d.%d.%d.%d:%d", a(), b(), c(), d(), m_port);
   return str;
}

uint8 ip_address::a() const
{
   return uint8(m_host >> 24);
}

uint8 ip_address::b() const
{
   return uint8(m_host >> 16);
}

uint8 ip_address::c() const
{
   return uint8(m_host >> 8);
}

uint8 ip_address::d() const
{
   return uint8(m_host & 0xff);
}
udp_socket::udp_socket()
   : m_handle(INVALID_SOCKET)
{
}

bool udp_socket::valid() const
{
   return m_handle != INVALID_SOCKET;
}

bool udp_socket::open()
{
   return open_and_bind({});
}

bool udp_socket::open_and_bind(const ip_address &address)
{
   SOCKET fd = ::socket(AF_INET, SOCK_DGRAM, 0);
   if (fd == INVALID_SOCKET) {
      return false;
   }

   sockaddr_in addr{};
   addr.sin_family = AF_INET;
   addr.sin_port = htons(address.m_port);
   addr.sin_addr.s_addr = htonl(address.m_host);
   if (::bind(fd, (const sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
      closesocket(fd);
      return false;
   }

   u_long non_blocking = 1;
   if (ioctlsocket(fd, FIONBIO, &non_blocking) == SOCKET_ERROR) {
      closesocket(fd);
      return false;
   }

   m_handle = fd;

   return valid();
}

void udp_socket::close()
{
   if (valid()) {
      closesocket(m_handle);
   }

   m_handle = INVALID_SOCKET;
}

bool udp_socket::send(const ip_address &address, const byte_stream &stream)
{
   if (!valid()) {
      return false;
   }

   const int32 length = stream.m_size;
   const char *data = stream.m_buffer;

   sockaddr_in addr{};
   addr.sin_family = AF_INET;
   addr.sin_port = htons(address.m_port);
   addr.sin_addr.s_addr = htonl(address.m_host);
   if (::sendto(m_handle, data, length, 0, (const sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
      return false;
   }

   return true;
}

bool udp_socket::receive(ip_address &address, byte_stream &stream)
{
   if (!valid()) {
      return false;
   }

   sockaddr_in addr{};
   int32 addrlen = sizeof(addr);
   int32 res = ::recvfrom(m_handle, stream.m_buffer, sizeof(stream.m_buffer), 0, (sockaddr *)&addr, &addrlen);
   if (res == SOCKET_ERROR) {
      return false;
   }

   address.m_host = ntohl(addr.sin_addr.s_addr);
   address.m_port = ntohs(addr.sin_port);
   stream.m_size  = res;

   return true;
}

bool udp_socket::get_bound_address(ip_address &address)
{
   if (!valid()) {
      return false;
   }

   sockaddr_in addr{};
   int addrlen = sizeof(addr);
   if (getsockname(m_handle, (sockaddr *)&addr, &addrlen) == SOCKET_ERROR) {
      return false;
   }

   address.m_host = ntohl(addr.sin_addr.s_addr);
   address.m_port = ntohs(addr.sin_port);

   return true;
}
