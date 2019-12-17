#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <future>
#include <utility>
#include <redisgraph.hpp>
#include <picojson/picojson.h>
static const std::string defaultHost("127.0.0.1");

TEST_CASE("Should send Graph query to redis", "[graph]")
{
    redisgraph::connection_context ctx{ defaultHost,6379, 2 };
    redisgraph::redis_executor executor(ctx);
    std::promise<redisgraph::result_view> promise;
    std::promise<redisgraph::result_view> promise1;
    executor.start();
    auto result = promise.get_future();
    auto result1 = promise1.get_future();
    bredis::single_command_t cmd("GRAPH.QUERY", "social2", "MATCH (n1)-[r]->(n2) RETURN n1, r, n2.name", "--compact");
    executor.send_command(cmd, std::move(promise));
    auto view = result.get();
    auto result_value = view.value();
    executor.shutdown();
}

