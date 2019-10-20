/**
 * @file   connection_context.hpp
 * @author Giorgio Zoppi (giorgio.zoppi@gmail.com)
 * @date   September, 2019
 * @version 1.0.0
 * @ingroup redisgraphcpp
 * @brief A graph class representing the real data contained in redisgraph
 *
 */
#ifndef REDIS_CONN_CONTEXT_H
#define REDIS_CONN_CONTEXT_H
#include <cstdint>

namespace redisgraph
{
    struct connection_context
    {
        uint32_t redis_port;
        std::string redis_host;   
    };
}
#endif
