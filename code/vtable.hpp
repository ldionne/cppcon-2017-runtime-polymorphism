// Copyright Louis Dionne 2017
// Distributed under the Boost Software License, Version 1.0.

#ifndef VTABLE_HPP
#define VTABLE_HPP

#include <cstddef>


// sample(vtable)
struct vtable {
  void (*accelerate)(void* this_);
  void (*dtor)(void* this_);
  void (*copy)(void* p, void const* other); // skip-sample
  std::size_t sizeof_;                      // skip-sample
};

template <typename T>
vtable const vtable_for = {
  [](void* this_) {
    static_cast<T*>(this_)->accelerate();
  },

  [](void* this_) {
    static_cast<T*>(this_)->~T();
  }
  ,                                           // skip-sample
  [](void* p, void const* other) {            // skip-sample
    new (p) T(*static_cast<T const*>(other)); // skip-sample
  },                                          // skip-sample
  sizeof(T)                                   // skip-sample
};
// end-sample

#endif // header guard
