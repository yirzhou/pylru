#ifndef INCLUDED_PYLRU_LRU_H_
#define INCLUDED_PYLRU_LRU_H_

#include <linked_list.h>

#include <iostream>
#include <unordered_map>
#include <vector>

namespace pylru {

template <typename Key, typename Value>
class lru {
  int capacity;
  linked_list<Key, Value> list;
  std::unordered_map<Key, node<Key, Value>*> mapping;

 public:
  lru(const int&);
  ~lru();
  Value get(const Key&);
  void put(const Key&, Value);
};

template <typename Key, typename Value>
lru<Key, Value>::lru(const int& capacity) : capacity(capacity) {}

template <typename Key, typename Value>
lru<Key, Value>::~lru() {}

template <typename Key, typename Value>
Value lru<Key, Value>::get(const Key& k) {
  if (this->mapping.find(k) == this->mapping.end()) {
    std::cout << "Can't find " << k << std::endl;
    return -1;
  }

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

    if (--this->capacity == -1) {
      n = this->list.delete_node(this->list.tail()->prev());
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

#endif
