/**
 * @file   node.hpp
 * @author Giorgio Zoppi <giorgio@apache.org>
 * @date   December, 2019
 * @version 1.0.0
 * @ingroup redisgraphcpp
 * @brief A node class that is representing the node in a graph db
 */

#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H
#include <string>
#include <memory>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace redisgraph
{
	/**
	* Node class.
	*/
	template <typename T> class node
	{
	public:
		explicit node(const std::string& label, const std::string& alias, const T& data) : label_(label), alias_(alias), data_(std::make_unique<T>(_data))
		{
		}
		explicit node(const node<T>& other) : label_(other.label_), alias_(other.alias_), id_(other.id_)
		{
			//Do assignment logic
			_data = std::make_unique<T>(other.data_)
		}
		node& operator=(node const& other)
		{
			if (this != &other)
			{
				this.label_ = other.label_;
				this.alias_ = other.alias_;
				this->id_ = other.id_;
				this.data_ = std::make_unique<T>(other.data_);
			}
			// Use copy and swap idiom to implement assignment.
			return *this;
		}

		bool operator==(const node& first, const node& second) noexcept
		{
			return (first.id_ == second.id_);
		}
		const boost::uuids::uuid id() const noexcept
		{
			return _id;
		}
		node(node&& that) noexcept
		{
			this->id = std::move(that.id_);
			this->name_ = std::move(that.name_);
			this->data_ = std::move(that.data_);
		}
		node& operator=(node&& that) noexcept
		{
			this->name_ = std::move(this->name_);
			this->data_ = std::move(this->data_);
			return *this;
		}
		bool operator==(const node& item)
		{
			return (id_ == item.id())
		}
		virtual ~node() = default;

	private:
		boost::uuids::uuid generate_id() const
		{
			boost::uuids::uuid u(boost::uuids::random_generator()());
			std::vector<char> v(u.size());
			std::copy(u.begin(), u.end(), v.begin());
			return u;

		}
		uint64_t id_;                /* Unique identifier of the node*/
		std::string label_;          /* Label of the  node */
		std::string alias_;          /* Alias of the node */
		std::unique_ptr<T> data_;    /* Data contained in the node */
	};
}
#endif