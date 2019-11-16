# redisgraph-cpp
RedisGraph C++ client

This is a simple project layout to start working.
If you're using a Linux machine, there is a Makefile wrapping a CMake project.
Please, use __make__ to build the system and __make distclean__ to delete the automatically generated files.
# Basic usage
```c++
    redisgraph::connection_context connection{"127.0.0.1", 6379};
    std::unique_ptr<redisgraph::graph> graph = std::make_unique<redisgraph::graph>("Collegues", connection);
    auto node1 = graph->add_node("Employee", "Liv Horowitz");
    auto node2 = graph->add_node("Company", "London Software");
    auto node3 = graph->add_node("Employee", "Danny Malley");
    auto node4 = graph->add_node("Employee", "Patricia Lutz");
    auto node5 = graph->add_node("Location", "London");
    graph->add_edge(node1, node2, "Works");
    graph->add_edge(node3, node2, "Works");
    graph->add_edge(node4, node2, "Works");
    graph->add_edge(node2, node5, "Located");
    // now we write the graph to redis
    auto commitResult = graph->commit_async();
    // we do something else.
    auto result = commitResult.get();
    if (result > 0) 
    {
        std::cout << "Graph created with success" << std::endl;
    }
```
