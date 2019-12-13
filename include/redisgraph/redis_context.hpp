#ifndef REDIS_UTILS_H 
#define REDIS_UTILS_H
#ifdef __OK__

#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <bredis.hpp>
namespace r = bredis;
namespace asio = boost::asio;
namespace sys = boost::system;
using socket_t = asio::ip::tcp::socket;
using next_layer_t = socket_t;
//using next_layer_t = r::test::SocketWithLogging<asio::ip::tcp::socket>;
using Buffer = boost::asio::streambuf;
using Iterator = typename r::to_iterator<Buffer>::iterator_t;
using Policy = r::parsing_policy::keep_result;
using result_t = r::positive_parse_result_t<Iterator, Policy>;
using Connection = r::Connection<next_layer_t>;

namespace redisgraph
{
    struct redis_accessor_t {
        Buffer tx_buff;
        Buffer rx_buff;
        Connection conn;
        asio::io_context& io;
        asio::io_context::strand strand;
        redis_accessor_t(socket_t&& s, asio::io_context& context) : conn{ std::move(s) }, io(context), strand(context){}
    };
}

#endif
#endif