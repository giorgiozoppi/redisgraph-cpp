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
#include <string>
#include <utility>
#include <redisgraph.hpp>
#include <picojson/picojson.h>
static const size_t NUMBER_THREADS = 4;
static const std::string defaultHost("127.0.0.1");
/*
redisgraph::graph<picojson::value>&& create_graph(const std::string& graph_name)
{
    redisgraph::connection_context ctx{ defaultHost,6379, NUMBER_THREADS };
   // redisgraph::graph<picojson::value> g(graph_name, ctx);
    return std::move(g);
}
*/
TEST_CASE("Graph should be start/stop correctly", "[graph]") {
    redisgraph::connection_context ctx{ defaultHost,6379, NUMBER_THREADS };
    redisgraph::graph<picojson::value> g("Social", ctx);
    g.start();
    g.ping();
    g.shutdown();
}
