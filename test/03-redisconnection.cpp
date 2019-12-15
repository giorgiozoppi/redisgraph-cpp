/**
* Copyright 2019 RRD Software Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
**/

#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <future>
#include <utility>
#include <redisgraph.hpp>
#include <picojson/picojson.h>
static const size_t NUMBER_THREADS = 4;
static const std::string defaultHost("127.0.0.1");

TEST_CASE("Should connect to the redis", "[graph]") {
    redisgraph::connection_context ctx{ defaultHost,6379, NUMBER_THREADS };
    redisgraph::redis_executor executor(ctx);
    std::vector<std::shared_future<redisgraph::result_view>> results;
    executor.start();
    auto started = executor.is_started();
    executor.shutdown();
    auto finished = executor.is_started();
    REQUIRE(started == true);
    REQUIRE(finished == true);
}
TEST_CASE("Should send multiple message to redis", "[graph]")
{
    redisgraph::connection_context ctx{ defaultHost,6379, 2};
    redisgraph::redis_executor executor(ctx);
    std::promise<redisgraph::result_view> result_promise;
    std::future<redisgraph::result_view> result_future = result_promise.get_future();
    executor.start();
    executor.send_message("PING", std::move(result_promise));
    auto view = result_future.get();
    auto myvalue = view.value();
    REQUIRE(myvalue == "PONG");
    executor.shutdown();
}

