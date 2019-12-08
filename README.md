# redisgraph-cpp
RedisGraph C++ client

This is a simple project layout to start working.
If you're using a Linux machine, there is a Makefile wrapping a CMake project.
Please, use __make__ to build the system and __make distclean__ to delete the automatically generated files.
# Basic usage
### Create a graph
```c++
    redisgraph::connection_context connection{"127.0.0.1", 6379};
    std::unique_ptr<redisgraph::graph> graph = std::make_unique<redisgraph::graph>("Collegues", connection);
    auto node1 = graph->add_node("First", "Luca Deri, Casablanca");
    auto node2 = graph->add_node("Second", "Simon perez");
    auto edge = graph->add_edge(node1, node2);
    // now we write the graph to redis
    auto future = graph->commit_async();
    // we do something else.
    auto result = future.get();
    if (result > 0) 
    {
        std::cout << "" << std::endl;
    }
```
### Query api


