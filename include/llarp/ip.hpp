#ifndef LLARP_IP_HPP
#define LLARP_IP_HPP
#include <llarp/buffer.h>
#include <llarp/time.h>
#include <llarp/net.hpp>

#ifndef _WIN32
// unix, linux
#include <sys/types.h>  // FreeBSD needs this for uchar for ip.h
#include <netinet/in.h>
#include <netinet/ip.h>
#else
// windows nt
#include <winsock2.h>
typedef struct ip_hdr
{
  unsigned char
      ip_header_len : 4;  // 4-bit header length (in 32-bit words) normally=5
                          // (Means 20 Bytes may be 24 also)
  unsigned char ip_version : 4;    // 4-bit IPv4 version
  unsigned char ip_tos;            // IP type of service
  unsigned short ip_total_length;  // Total length
  unsigned short ip_id;            // Unique identifier

  unsigned char ip_frag_offset : 5;  // Fragment offset field

  unsigned char ip_more_fragment : 1;
  unsigned char ip_dont_fragment : 1;
  unsigned char ip_reserved_zero : 1;

  unsigned char ip_frag_offset1;  // fragment offset

  unsigned char ip_ttl;        // Time to live
  unsigned char ip_protocol;   // Protocol(TCP,UDP etc)
  unsigned short ip_checksum;  // IP checksum
  unsigned int ip_srcaddr;     // Source address
  unsigned int ip_destaddr;    // Source address
} IPV4_HDR;
#define iphdr IPV4_HDR
#define saddr ip_srcaddr
#define daddr ip_destaddr
#define check ip_checksum
#define ihl ip_header_len
#define ip_version version
#endif

#include <memory>

// anything not win32
struct ip_header
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
  unsigned int ihl : 4;
  unsigned int version : 4;
#elif __BYTE_ORDER == __BIG_ENDIAN
  unsigned int version : 4;
  unsigned int ihl : 4;
#else
#error "Please fix <bits/endian.h>"
#endif

#if defined(__linux__)
#define ip_version version
#endif
  uint8_t tos;
  uint16_t tot_len;
  uint16_t id;
  uint16_t frag_off;
  uint8_t ttl;
  uint8_t protocol;
  uint16_t check;
  uint32_t saddr;
  uint32_t daddr;
};

namespace llarp
{
  namespace net
  {
    struct IPv4Packet
    {
      static constexpr size_t MaxSize = 1500;
      llarp_time_t timestamp;
      size_t sz;
      byte_t buf[MaxSize];

      llarp_buffer_t
      Buffer();

      bool
      Load(llarp_buffer_t buf);

      struct GetTime
      {
        llarp_time_t
        operator()(const IPv4Packet& pkt) const
        {
          return pkt.timestamp;
        }
      };

      struct PutTime
      {
        void
        operator()(IPv4Packet& pkt) const
        {
          pkt.timestamp = llarp_time_now_ms();
        }
      };

      struct CompareOrder
      {
        bool
        operator()(const IPv4Packet& left, const IPv4Packet& right)
        {
          return left.timestamp < right.timestamp;
        }
      };

      ip_header*
      Header()
      {
        return (ip_header*)&buf[0];
      }

      const ip_header*
      Header() const
      {
        return (ip_header*)&buf[0];
      }

      uint32_t
      src()
      {
        return ntohl(Header()->saddr);
      }

      uint32_t
      dst()
      {
        return ntohl(Header()->daddr);
      }

      void
      src(uint32_t ip)
      {
        Header()->saddr = htonl(ip);
      }

      void
      dst(uint32_t ip)
      {
        Header()->daddr = htonl(ip);
      }

      // update ip packet checksum
      void
      UpdateChecksum();
    };

  }  // namespace net
}  // namespace llarp

#endif
