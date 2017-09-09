// Copyright Louis Dionne 2017
// Distributed under the Boost Software License, Version 1.0.

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>


// sample(vtable)
struct vtable {
  void (*accelerate)(void* this_);
  void (*dtor)(void* this_);
  void (*copy)(void* p, void const* other); // skip-sample
  std::size_t sizeof_;                      // skip-sample
};

template <typename T>
vtable const vtable_for = {
  // accelerate()
  [](void* this_) {
    static_cast<T*>(this_)->accelerate();
  },

  // destructor
  [](void* this_) {
    static_cast<T*>(this_)->~T();
  }
  ,                                           // skip-sample
  // copy-constructor                         // skip-sample
  [](void* p, void const* other) {            // skip-sample
    new (p) T(*static_cast<T const*>(other)); // skip-sample
  },                                          // skip-sample
  sizeof(T)                                   // skip-sample
};
// end-sample

// sample(Vehicle)
struct Vehicle {
  template <typename Any>
    // enabled only when vehicle.accelerate() is valid
  Vehicle(Any vehicle)
    : vptr_{&vtable_for<Any>}
    , impl_{std::malloc(sizeof(Any))}
  { new (impl_) Any{vehicle}; }
                                                      // skip-sample
  Vehicle(Vehicle const& other)                       // skip-sample
    : vptr_{other.vptr_}                              // skip-sample
    , impl_{std::malloc(other.vptr_->sizeof_)}        // skip-sample
  {                                                   // skip-sample
    other.vptr_->copy(impl_, other.impl_);            // skip-sample
  }                                                   // skip-sample

  void accelerate()
  { vptr_->accelerate(impl_); }

  ~Vehicle() {
    vptr_->dtor(impl_);
    std::free(impl_);
  }

private:
  vtable const* vptr_;
  void* impl_;
};
// end-sample


//////////////////////////////////////////////////////////////////////////////
struct Car {
  std::string make;
  int year;
  void accelerate() { std::cout << "Car::accelerate()" << std::endl; }
};

struct Truck {
  std::string make;
  int year;
  void accelerate() { std::cout << "Truck::accelerate()" << std::endl; }
};

struct Plane {
  std::string make;
  std::string model;
  void accelerate() { std::cout << "Plane::accelerate()" << std::endl; }
};

// sample(main)
int main() {
  std::vector<Vehicle> vehicles;

  vehicles.push_back(Car{"Audi", 2017});
  vehicles.push_back(Truck{"Chevrolet", 2015});
  vehicles.push_back(Plane{"Boeing", "747"});

  for (auto& vehicle : vehicles) {
    vehicle.accelerate();
  }
}
// end-sample
