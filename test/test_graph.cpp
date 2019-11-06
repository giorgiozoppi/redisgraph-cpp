
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"
#include "redisgraph/graph.hpp"

static const size_t NUMBER_THREADS = 4;	

redisgraph::graph create_social_graph()
{
    redisgraph::connection_context ctx('127.0.0.1','6379', NUMBER_THREADS);
	redisgraph::graph<std::string> g("Social",ctx);
}
TEST_CASE( "Graph should be empty initalized correctly", "[graph]" ) {
    redisgraph::connection_context ctx('127.0.0.1','6379', NUMBER_THREADS);
	redisgraph::graph g("Social",ctx);
    REQUIRE( g.name() == "Test" );
    REQUIRE( g.num_nodes() == 0 );
}
TEST_CASE("Nodes should be created correctly","[graph]")
{
    const size_t NUMBER_THREADS = 4;
    redisgraph::connection_context ctx('127.0.0.1','6379', NUMBER_THREADS);
	redisgraph::graph g("MotoGP",ctx);
    auto src = g.add_node<std::string>(node("Rider","{name: 'Valentino Rossi'}");
    auto dst = g.add_node<std::string>(node("Team","{name: 'Yamaha'}"));
    g.add_edge("rides", src, dst);
    src = g.add_node(node("Rider","{name: Dani Pedrosa}"));
    dst = g.add_node(node("Team", "{name: Honda}"));
    g.add_edge("rides", src, dst);
    auto saved = g.commit_async();
    REQUIRE(saved == true)    
};
TEST_CASE("Graph api should execute opencypher correctly", "[graph]")
{
 redisgraph::connection_context ctx('127.0.0.1','6379', NUMBER_THREADS);
 redisgraph::graph api("social",ctx);  
 std::future<redisgraph::resultset> result = api.query_async("social", "CREATE ({name:'simon',age:42})");
 result.get();
 REQUIRE( api.num_nodes() == 1 );
}
TEST_CASE("Graph api should execute opencypher with connecting nodes","[graph]")
{
    redisgraph::connection_context ctx('127.0.0.1','6379', NUMBER_THREADS);
    redisgraph::graph api("social",ctx);  
    auto firstquery =  api.query("social", "CREATE (:person{name:'simon',age:42})"));
    auto secondquery = api.query("social", "CREATE (:person{name:'lukas',age:18})"));
    secondquery.get();
    firstquery.get();
    // Connect source and destination nodes.
   auto selection = api.query("social", "MATCH (a:person), (b:person) WHERE (a.name = 'roi' AND b
 CREATE (a)-[:knows]->(b)");

    Assert.assertFalse(resultSet.hasNext());
}
}

