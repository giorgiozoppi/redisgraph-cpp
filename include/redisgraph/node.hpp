#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H
#include <memory>
#include <siphash.h>
#include <string>
namespace redisgraph {

/**
 * Class
 */
template <typename T> class node {

public:
  node(const std::string &name, const T &data)
      : name_(name), data_(std::make_unique<T>(_data)) {}
  node(node const& other): name_(other.name)
        {
            //Do assignment logic
            _data = std::make_unique<T>(other.data_.get())
        }
        node &operator=(node const& other) 
        {
    if (this != &other) {
      this.name_ = other.name_;
      this.data_ = std::make_unique<T>(other.data_.get());
    }
    // Use copy and swap idiom to implement assignment.
    return *this;
  }
  bool operator==(const node &first, const node &second) {
    return (first.name == second.name);
  }
  node(node &&that) noexcept {
    this->name_ = std::move(that.name_);
    this->data_ = std::move(that.data_);
  }
  node &operator=(node &&that) noexcept {
    this->name_ = std::move(this->name_);
    this->data_ = std::move(this->data_);
    return *this;
  }
  bool operator==(const node &item) { return (id_ == item.name_) }
  virtual ~node() {}

private:
  std::string name_;        /* Label of the  node */
  std::unique_ptr<T> data_; /* Data contained in the node */
};
} // namespace redisgraph
#endif