#include <linked_list.h>

namespace pylru {
template <typename Key, typename Value>
linked_list<Key, Value>::linked_list() {
  this->shead = new node<Key, Value>(0, 0, nullptr, nullptr);
  this->stail = new node<Key, Value>(0, 0, nullptr, nullptr);
  this->shead->next() = this->stail;
  this->stail->prev() = this->shead;
}

template <typename Key, typename Value>
linked_list<Key, Value>::~linked_list() {
  if (this->shead) delete this->shead;
  if (this->stail) delete this->stail;
}

template <typename Key, typename Value>
node<Key, Value>* linked_list<Key, Value>::delete_node(
    node<Key, Value>* listnode) {
  node<Key, Value>*prevnode = listnode->prev(), nextnode = listnode->next();
  prevnode->next() = nextnode;
  nextnode->prev() = prevnode;

  listnode->prev() = nullptr;
  listnode->next() = nullptr;

  return listnode;
}

template <typename Key, typename Value>
void linked_list<Key, Value>::insert_node(node<Key, Value>* listnode) {
  node<Key, Value>* headnext = this->shead->next();
  headnext->prev() = listnode;
  this->shead->next() = listnode;
  listnode->prev() = this->shead;
  listnode->next() = headnext;
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
