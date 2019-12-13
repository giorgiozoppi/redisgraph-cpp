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

TEST_CASE("Graph should be able to add an edge")
{
    redisgraph::connection_context ctx{ defaultHost,6379, NUMBER_THREADS };
    redisgraph::graph<picojson::value> g("Social", ctx);
    std::string firstNodeStr = "{ \"name\": \"Luis\", \"surname\": \"Moreno\",\"age\": 33 }";
    std::string secondNodeStr = "{ \"name\": \"Japan\"}";
    auto first = g.add_node("Person", "", redisgraph::make_node_data(firstNodeStr).value());
    auto second = g.add_node("Country", "", redisgraph::make_node_data(secondNodeStr).value());
    auto currentEdge = g.add_edge("VISIT", first.value(), second.value(), "{\"purpose\": \"pleasure\"}").value();
    auto edge_nodes = g.get_edges(first.value());
    auto edge = std::find_if(edge_nodes.begin(), edge_nodes.end(), [&currentEdge](const redisgraph::edge<picojson::value>& e)
        { return e.id() == currentEdge.id(); });
    REQUIRE(edge != edge_nodes.end());
}

