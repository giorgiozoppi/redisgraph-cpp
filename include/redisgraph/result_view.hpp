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

#ifndef REDISGRAPH_CPP_RESULT_VIEW_H
#define REDISGRAPH_CPP_RESULT_VIEW_H
#include <string>
#include <future>
#include <memory>
#include <optional>
#include <utility>
#include <thread>
#include <random>
#include <iostream>

namespace redisgraph {

	enum class result_view_coltype_t {
		COLUMN_UNKNOWN = 0,
		COLUMN_SCALAR = 1,
		COLUMN_NODE = 2,
		COLUMN_RELATION = 3,
	};
	enum class result_view_scalartype_t 
	{
		VALUE_UNKNOWN = 0,
		VALUE_NULL = 1,
		VALUE_STRING = 2,
		VALUE_INTEGER = 3,
		VALUE_BOOLEAN = 4,
		VALUE_DOUBLE = 5,
		VALUE_ARRAY = 6,
		VALUE_EDGE = 7,
		VALUE_NODE = 8,
		VALUE_PATH = 9
	};

	class result_view
	{
	public:
		result_view() = default;

		result_view(const std::string& result) : value_(result)
		{
			parse();
		}

		std::string value() const noexcept
		{
			return value_;
		}
		void parse_score() {}
		double score() const noexcept
		{
			 std::random_device rd;  //Will be used to obtain a seed for the random number engine
    		 std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    		 std::uniform_real_distribution<> dis(0.1, 1.0);
			 return dis(gen);
		}
		bool valid() const
		{
			return valid_;
		}
	private:
		std::string value_;
		bool valid_;
		void parse()
		{
		//    auto extract = boost::apply_visitor(redis::extractor<Iterator>(), view);
		//	auto& reply = boost::get<redis::extracts::string_t>(extract);
			//	value_ = reply.str;
		}

    };
}
#endif
