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

#ifndef REDISGRAPH_CPP_NODE_H
#define REDISGRAPH_CPP_NODE_H
#include <string>
#include <cstdint>
#include <memory>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "siphash.hpp"

namespace redisgraph
{
	using namespace boost::uuids;
	
	
	/**
	* Graph node class
	*/
	template <typename T> class node
	{
	public:

		node() = default;
		/**
		* Constructor. An unique identifier gets generated.
		* @param label label of the node
		* @param alias alias of the node
		* @param data value of the node (it might be a JSON or binary data)
		*/
		explicit node(const uint64_t id, const std::string& label, const std::string& alias, const T& data) : id_(id), label_(label), alias_(alias), data_(std::make_unique<T>(data))
		{
		}

		/**
		* Copy constructor
		*/
		explicit node(const node<T>& other) : id_(other.id_),label_(other.label_), alias_(other.alias_)
		{
			//Do assignment logic

			data_ = std::make_unique<T>(*other.data_);
			
		}
		/**
		* Copy assignment constructor
		*/
		node<T>& operator=(node<T> const& other)
		{
			if (this != &other)
			{
				this->label_ = other.label_;
				this->alias_ = other.alias_;
				this->id_ = other.id_;
				this->data_ = std::make_unique<T>(*other.data_);
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
		const std::string alias() const noexcept
		{
			return alias_;
		}
		const std::string label() const noexcept
		{
			return label_;
		}

		const T data() const noexcept
		{
			return *data_;
		}

		std::string str(const std::string& properties = "") const
		{
			std::ostringstream nodestream;
			nodestream << "(";
			if (!this->alias_.empty())
			{
				nodestream << this->alias_;
			}
			if (!this->label_.empty())
			{
				nodestream << ':' + self.label;
			}
			if (!properties.empty())
			{
				nodestream << properties;
			}
			nodestream << ")";
			return nodestream.str();
		}

		node(node&& that) noexcept
		{
			this->id_ = that.id_;
			this->label_ = std::move(that.label_);
			this->alias_ = std::move(that.alias_);
			this->data_ = std::move(that.data_);
		}
		node& operator=(node&& that) noexcept
		{
			this->id_ = that.id_;
			this->label_ = std::move(that.label_);
			this->alias_ = std::move(that.alias_);
			this->data_ = std::move(that.data_);
			return *this;
		}
		
		virtual ~node() = default;

	private:
		uint64_t id_;      /* Unique identifier of the node*/
		std::string label_;          /* Label of the  node */
		std::string alias_;          /* Alias of the node */
		std::unique_ptr<T> data_;    /* Data contained in the node */
	};
	template <typename T> std::unique_ptr<node<T>> make_unique_node(const std::string& label, 
		const std::string& alias, 
		const T& data, 
		size_t data_size)
	{
		redisgraph::siphash hashFunction;
		void* uuid_data = std::calloc(data_size, sizeof(uint8_t));
		std::memcpy(uuid_data, &data, data_size);
		hashFunction.set_data(uuid_data, data_size);
		auto id = hashFunction.computeHash();
		std::free(uuid_data);
		return std::make_unique <node<T>>(id, label, alias, data);
	};
}
#endif