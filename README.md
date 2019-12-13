# redisgraph-cpp
RedisGraph C++ client

This is a simple project layout to start working.
If you're using a Linux machine, there is a Makefile wrapping a CMake project.
Please, use __make__ to build the system and __make distclean__ to delete the automatically generated files.
# Basic usage
### Create a graph
```c++
    redisgraph::connection_context connection{"127.0.0.1", 6379, 4};
    redisgraph::graph<picojson::value> g("Social", ctx);
    std::string firstNodeStr = "{ \"name\": \"Luis\", \"surname\": \"Moreno\",\"age\": 33 }";
    std::string secondNodeStr = "{ \"name\": \"Japan\"}";
    auto first = g.add_node("Person", "", redisgraph::make_node_data(firstNodeStr).value());
    auto second = g.add_node("Country", "", redisgraph::make_node_data(secondNodeStr).value());
    auto currentEdge = g.add_edge("VISIT", first.value(), second.value(), "{\"purpose\": \"pleasure\"}").value();
    auto edge_nodes = g.get_edges(first.value());
    auto edge = std::find_if(edge_nodes.begin(), edge_nodes.end(), [&currentEdge](const redisgraph::edge<picojson::value>& e)
        { return e.id() == currentEdge.id(); });
    // now we write the graph to redis
    auto commitResult = graph->commit_async();
    // we do something else.
    auto result = commitResult.get();
    if (result > 0) 
    {
        std::cout << "Graph created with success" << std::endl;
    }
```
### Query api. Executing cypher.
```c++
const int NUMBER_THREADS = 4;
 redisgraph::connection_context ctx('127.0.0.1','6379', NUMBER_THREADS);
 redisgraph::graph api("social",ctx);  
 auto results = api.query_async("MATCH (a:person), (b:person) WHERE (a.name = 'roi' AND b CREATE (a)-[:knows]->(b)");
 use_results(results);
```

