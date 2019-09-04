/**
 * @file   graph.hpp
 * @author Josep Sabater (pep.sabater@gmail.com)
 * @date   September, 2019
 * @version 1.0.0
 * @ingroup redisgraphcpp
 * @brief A graph class representing the real data contained in redisgraph
 *
 */
#ifndef REDISGRAPH_CPP_GRAPH_H_
#define REDISGRAPH_CPP_GRAPH_H_

#include <string>

namespace redisgraphcpp {

	class Graph {

		private:
			std::string name_;
			int num_nodes_;

		public:
			Graph(const std::string &name) 
					: name_{name} 
					, num_nodes_{0} {}
					
			inline std::string Name() const { return name_; }

			inline int NumNodes() const { return  num_nodes_; }
			
			/* Not implemented yet. This is just an example*/
			int AddNode();
			
			/* Not implemented yet. This is just an example*/
			int AddEdge();
	};
}

#endif /* REDISGRAPH_CPP_GRAPH_H_ */