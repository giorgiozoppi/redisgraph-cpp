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
static const std::string defaultHost("127.0.0.1");

TEST_CASE("Should send multiple message to redis", "[graph]")
{
    redisgraph::connection_context ctx{ defaultHost,6379, 2};
    redisgraph::redis_executor executor(ctx);
       std::vector<std::promise<redisgraph::result_view>> result_promises;
        std::vector<std::shared_future<redisgraph::result_view>> results;

        for (int i = 0; i < 10; i++)
        {
            std::promise<redisgraph::result_view> result_promise;
            result_promises.emplace_back(std::move(result_promise));
        }
        
        executor.start();
        for (auto&& promise : result_promises)
        {
            auto current = promise.get_future();
            results.emplace_back(std::move(current));
            executor.send_message("PING", std::move(promise));
        }
        bool entered = false;
        for (auto&& result : results)
        {
            auto view = result.get();
            auto result_value = view.value();
            entered = true;
            if (result_value != "PONG")
            {
                executor.shutdown();
            }
            REQUIRE(result_value == "PONG");
        }
        REQUIRE(entered == true);
    executor.shutdown();
}

