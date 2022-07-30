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
  node<Key, Value>* delete_node(node<Key, Value>* listnode);
  void insert_node(node<Key, Value>* listnode);
  node<Key, Value>* head();
  node<Key, Value>* tail();
};

template <typename Key, typename Value>
linked_list<Key, Value>::linked_list() {
  this->shead = new node<Key, Value>(0, 0, nullptr, nullptr);
  this->stail = new node<Key, Value>(0, 0, nullptr, nullptr);
  this->shead->set_next(this->stail);
  this->stail->set_prev(this->shead);
}

template <typename Key, typename Value>
linked_list<Key, Value>::~linked_list() {
  node<Key, Value>* head = this->shead;
  while (head != nullptr) {
    node<Key, Value>* tmp = head->next();
    delete head;
    head = tmp;
  }
}

template <typename Key, typename Value>
node<Key, Value>* linked_list<Key, Value>::delete_node(
    node<Key, Value>* listnode) {
  node<Key, Value>* prevnode = listnode->prev();
  node<Key, Value>* nextnode = listnode->next();
  prevnode->set_next(nextnode);
  nextnode->set_prev(prevnode);

  listnode->set_prev(nullptr);
  listnode->set_next(nullptr);

  return listnode;
}

template <typename Key, typename Value>
void linked_list<Key, Value>::insert_node(node<Key, Value>* listnode) {
  node<Key, Value>* headnext = this->shead->next();
  headnext->set_prev(listnode);
  this->shead->set_next(listnode);
  listnode->set_prev(this->shead);
  listnode->set_next(headnext);
}

template <typename Key, typename Value>
node<Key, Value>* linked_list<Key, Value>::head() {
  return this->shead;
}

template <typename Key, typename Value>
node<Key, Value>* linked_list<Key, Value>::tail() {
  return this->stail;
}

}  // namespace pylru

#endif
