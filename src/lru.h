#ifndef INCLUDED_PYLRU_LRU_H_
#define INCLUDED_PYLRU_LRU_H_

#include <linked_list.h>

#include <unordered_map>
#include <vector>

namespace pylru {

template <typename Key, typename Value>
class lru {
  int capacity;
  linked_list<Key, Value> list;
  std::unordered_map<Key, Value> mapping;

 public:
  lru(const int&);
  ~lru();
  Value get(const Key&);
  void put(const Key&, Value);
};

}  // namespace pylru

#endif
