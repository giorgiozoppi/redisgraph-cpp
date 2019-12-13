#ifndef JSON_NODE_HPP
#define JSON_NODE_HPP
#include <memory.h>
#include <cstdint>
#include <string>
#include <picojson/picojson.h>
#include <redisgraph/siphash.hpp>
#include <redisgraph/node.hpp>
namespace redisgraph
{
	template <> std::unique_ptr<node<picojson::value>> make_unique_node(const std::string& label,
		const std::string& alias,
		const picojson::value& data,
		size_t data_size)
	{
		redisgraph::siphash hashFunction;
		auto str = data.serialize();
		hashFunction.set_data(str.c_str(), str.size());
		auto id = hashFunction.computeHash();
		return std::make_unique<node<picojson::value>>(id, label, alias, data);
	};
}
#endif