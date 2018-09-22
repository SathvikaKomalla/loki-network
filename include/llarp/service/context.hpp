#ifndef LLARP_SERVICE_CONTEXT_HPP
#define LLARP_SERVICE_CONTEXT_HPP
#include <llarp/router.h>
#include <llarp/service/config.hpp>
#include <llarp/service/endpoint.hpp>
#include <unordered_map>
#include <llarp/handlers/tun.hpp>

namespace llarp
{
  namespace service
  {
    /// holds all the hidden service endpoints we own
    struct Context
    {
      Context(llarp_router *r);
      ~Context();

      void
      Tick();

      /// DRY refactor
      llarp::service::Endpoint *
      getFirstEndpoint();

      /// DRY refactor
      llarp::handlers::TunEndpoint *
      getFirstTun();

      /// punch a hole to get ip range from first tun endpoint
      llarp_tun_io *
      getRange();

      /// hint at possible path usage and trigger building early
      bool
      Prefetch(const llarp::service::Address &addr);

      /// punch a hole open for DNS to add mappings
      bool
      MapAddress(const llarp::service::Address &addr, uint32_t ip);

      bool
      AddEndpoint(const Config::section_t &conf);

     private:
      llarp_router *m_Router;
      std::unordered_map< std::string, std::unique_ptr< Endpoint > >
          m_Endpoints;
    };
  }  // namespace service
}  // namespace llarp
#endif
