#ifndef _ITERPRIOQUEUE_H_
#define _ITERPRIOQUEUE_H_

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <queue>

template <class T, class Compare>
class IterPrioQueue : public std::vector<T> {
  Compare comp;

 public:
  IterPrioQueue(Compare cmp = Compare()) : comp(cmp) { refresh(); }
  const T& top() { return this->front(); }
  void push(const T& x) {
    this->push_back(x);
    push_heap(this->begin(), this->end(), comp);
  }
  void pop() {
    pop_heap(this->begin(), this->end(), comp);
    this->pop_back();
  }

  void refresh() { make_heap(this->begin(), this->end(), comp); }
};

/*
 Taken from
 http://www.linuxtopia.org/online_books/programming_books/c++_practical_programming/c++_practical_programming_189.html
*/

#endif