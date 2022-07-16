#include <node.h>

namespace pylru {
template <typename Key, typename Value>
node<Key, Value>::node(const Key& k, Value val, node* prev_node,
                       node* next_node)
    : key(k), val(val) {
  this->prev_node = prev_node;
  this->next_node = next_node;
}

template <typename Key, typename Value>
node<Key, Value>::~node() {
  if (this->prev_node) delete this->prev_node;
  if (this->next_node) delete this->next_node;
}

template <typename Key, typename Value>
bool node<Key, Value>::set(const Key& k, Value v) {
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
}  // namespace pylru
