#ifndef INCLUDED_PYLRU_NODE_H_
#define INCLUDED_PYLRU_NODE_H_

namespace pylru {

template <typename Key, typename Value>
class node {
  node* prev_node;
  node* next_node;
  Key key;
  Value val;

 public:
  node(const Key&, Value, node*, node*);
  ~node();
  bool set(const Key&, Value);
  node* prev();
  node* next();
  Key k();
  Value v();
};

}  // namespace pylru

#endif
