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

#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H
#include <string>
#include <cstdint>
#include <memory>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "siphash.hpp"

namespace redisgraph
{
	/**
	* Graph node class
	*/
	template <typename T> class node
	{
	public:
		/**
		* Constructor. An unique identifier gets generated.
		* @param label label of the node
		* @param  alias alias of the node
		* @param data value of the node (it might be a JSON or binary data)
		*/
		explicit node(const std::string& label, const std::string& alias, const T& data) : label_(label), alias_(alias), data_(std::make_unique<T>(_data))
		{
			_id = generate_id();
		}
		/**
		* Copy constructor
		*/
		explicit node(const node<T>& other) : label_(other.label_), alias_(other.alias_), id_(other.id_)
		{
			//Do assignment logic
			_data = std::make_unique<T>(other.data_)
		}
		/**
		* Copy assignment constructor
		*/
		node& operator=(node const& other)
		{
			if (this != &other)
			{
				this->label_ = other.label_;
				this->alias_ = other.alias_;
				this->id_ = other.id_;
				this->data_ = std::make_unique<T>(other.data_);
			}
			// Use copy and swap idiom to implement assignment.
			return *this;
		}
		/**
		* Equality operator
		*/
		bool operator==(const node& first) noexcept
		{
			return (this->id() == first.id_);
		}
		/**
		* Get an identifier
		*/
		const uint64_t id() const noexcept
		{
			return id_;
		}
		node(node&& that) noexcept
		{
			this->id = std::move(that.id_);
			this->label_ = std::move(that.label_);
			this->alias_ = std::move(other.alias_);
			this->data_ = std::move(that.data_);
		}
		node& operator=(node&& that) noexcept
		{
			this->id = std::move(that.id_);
			this->label_ = std::move(that.label_);
			this->alias_ = std::move(other.alias_);
			this->data_ = std::move(that.data_);
			return *this;
		}
		
		virtual ~node() = default;

	private:
		uint64_t generate_id() const
		{
			redisgraph::siphash hashFunction;
			boost::uuids::uuid u(boost::uuids::random_generator()());
			uint64_t uuid_data[16];
			std::memcpy(uuid_data, &u, 16);
			hashFunction.set_data(uuid_data)
			return hashFunction.computeHash();

		}
		uint64_t id_;      /* Unique identifier of the node*/
		std::string label_;          /* Label of the  node */
		std::string alias_;          /* Alias of the node */
		std::unique_ptr<T> data_;    /* Data contained in the node */
	};
}
#endif