#include <lru.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace pylru {
template <typename Key, typename Value>
lru<Key, Value>::lru(const int capacity) : capacity(capacity) {}

template <typename Key, typename Value>
lru<Key, Value>::~lru() {}

template <typename Key, typename Value>
Value lru<Key, Value>::get(const Key& k) {
  if (this->mapping.find(k) == this->mapping.end()) return nullptr;

  node<Key, Value>* n = this->list.delete_node(this->mapping.at(k));
  this->list.insert_node(n);
  return n->v();
}

template <typename Key, typename Value>
void lru<Key, Value>::put(const Key& k, Value val) {
  if (this->mapping.find(k) == this->mapping.end()) {
    node<Key, Value>* n = new node<Key, Value>(k, val, nullptr, nullptr);
    this->list.insert_node(n);
    this->mapping[k] = n;
    if (--this->capacity) {
      n = this->list.delete_node(this->list->tail()->prev());
      this->mapping.erase(n->k());
      this->capacity++;
    }
  } else {
    node<Key, Value>* n = this->list.delete_node(this->mapping[k]);
    n->set(n->k(), val);
    this->list.insert_node(n);
  }
}

}  // namespace pylru
