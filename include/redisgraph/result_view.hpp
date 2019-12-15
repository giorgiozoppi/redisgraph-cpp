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

namespace redisgraph {

	class result_view
	{
	public :
		result_view() = default;

		result_view(const std::string& result): value_(result)
		{
			parse();
		}
		std::string value() const noexcept
		{
			return value_;
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
		//	auto extract = boost::apply_visitor(redis::extractor<Iterator>(), view);
			//	auto& reply = boost::get<redis::extracts::string_t>(extract);
			//	value_ = reply.str;
		}

    };
}
#endif
