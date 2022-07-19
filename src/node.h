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
  void set(const Key&, Value);
  node* prev();
  node* next();
  void set_next(node* next);
  void set_prev(node* prev);
  Key k();
  Value v();
};

template <typename Key, typename Value>
node<Key, Value>::node(const Key& k, Value val, node* prev_node,
                       node* next_node)
    : key(k), val(val) {
  this->prev_node = prev_node;
  this->next_node = next_node;
}

template <typename Key, typename Value>
node<Key, Value>::~node() {}

template <typename Key, typename Value>
node<Key, Value>* node<Key, Value>::prev() {
  return this->prev_node;
}

template <typename Key, typename Value>
node<Key, Value>* node<Key, Value>::next() {
  return this->next_node;
}

template <typename Key, typename Value>
void node<Key, Value>::set(const Key& k, Value v) {
  this->key = k;
  this->val = v;
}

template <typename Key, typename Value>
Key node<Key, Value>::k() {
  return this->key;
}

template <typename Key, typename Value>
Value node<Key, Value>::v() {
  return this->val;
}

template <typename Key, typename Value>
void node<Key, Value>::set_prev(node<Key, Value>* prev) {
  this->prev_node = prev;
}

template <typename Key, typename Value>
void node<Key, Value>::set_next(node<Key, Value>* next) {
  this->next_node = next;
}

}  // namespace pylru

#endif
