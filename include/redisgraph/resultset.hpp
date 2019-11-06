#ifndef REDIS_GRAPH_RESULT_SET_H
#define REDIS_GRAPH_RESULT_SET_H
namespace redisgraph {
struct result_set;
class result_set_iterator {
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = result_set;
  using difference_type = result_set;
  using pointer = result_set *;
  using reference = result_set &;
  result_set_iterator(result_set *ptr = nullptr) { m_ptr = ptr; }
  result_set_iterator(const result_set_iterator &rawIterator) = default;
  ~result_set_iterator() {}

private:
  result_set *m_ptr;
};
template <> struct iterator_traits<result_set_iterator> {
  typedef typename result_set_iterator::value_type value_type;
  typedef typename result_set_iterator::pointer *pointer;
  typedef typename result_set_iterator::difference_type difference_type;
  typedef typename result_set_iterator::reference reference;
};

struct result_set {
  node next();
  ;
}
#endif
