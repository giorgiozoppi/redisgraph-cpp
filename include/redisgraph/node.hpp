#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <string>
#include <memory>
#include <boost
#include <siphash.hpp>
#include <boost/uuid.hpp>

namespace redisgraph
{
   
    /**
    * Class
    */
    template <typename T> class node 
    {

        public:
        node(const std::string& name, const T& data): name_(name), data_(std::make_unique<T>(_data))
        {

        }
        node(node const& other): name_(other.name)
        {
            //Do assignment logic
            _data = std::make_unique<T>(other.data_)
        }

        node &operator=(node const& other) 
        {
            if(this != &other)
            {
                this.name_ = other.name_;
                this.data_ = std::make_unique<T>(other.data_);
            }
        // Use copy and swap idiom to implement assignment.
            return *this;
        }
        node(node&& other)
        {
            
        }
    bool operator==(const node& first, const node& second)
    {
        return (first.name == second.name);
    }
    const uint64_t id() const 
    {
        return _id;
    }   
    node(node&& that) noexcept
    {
        this->name_ = std::move(that.name_);
        this->data_= std::move(that.data_);
    }
    node& operator=(node&& that) noexcept
    {
        this->name_ = std::move(this->name_);
        this->data_ = std::move(this->data_);
        return *this;
    }
    bool operator==(const node& item)
    {
        return (id_ == item.name_)
    }
        virtual ~node() {}

        private:
        uint64_t generate_id() const
        {
            boost::uuids::uuid u;
            std::vector<char> v(u.size());
            std::copy(u.begin(), u.end(), v.begin());

        }
        uint64_t id;
        std::string name_;          /* Label of the  node */
        std::unique_ptr<T> data_;   /* Data contained in the node */
    };
}
#endif