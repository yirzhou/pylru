#ifndef INCLUDED_PYLRU_LIST_H_
#define INCLUDED_PYLRU_LIST_H_

#include <node.h>

namespace pylru {
template <typename Key, typename Value>
class linked_list {
  node<Key, Value>* shead;
  node<Key, Value>* stail;

 public:
  linked_list();
  ~linked_list();
  static node<Key, Value>* delete_node(node<Key, Value>* listnode);
  node<Key, Value>* head();
  node<Key, Value>* tail();
  void insert_node(node<Key, Value>* listnode);
};
}  // namespace pylru

#endif
