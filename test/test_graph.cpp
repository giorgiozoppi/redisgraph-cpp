
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"
#include "redisgraph-cpp/graph.hpp"

TEST_CASE( "Graph is created correctly", "[graph]" ) {
	
	redisgraphcpp::Graph g("Test");
    REQUIRE( g.Name() == "Test" );
    REQUIRE( g.NumNodes() == 0 );
    REQUIRE( g.AddNode() > 0 );
    REQUIRE( g.AddEdge() > 0 );	
}
