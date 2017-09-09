// Copyright Louis Dionne 2017
// Distributed under the Boost Software License, Version 1.0.

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>


// sample(vtable)
struct vtable {
  void (*dtor)(void* this_);
  // ...
  void (*copy)(void* p, void const* other); // skip-sample
  std::size_t sizeof_;                      // skip-sample
};

template <typename T>
vtable const vtable_for = {
  [](void* this_) {
    static_cast<T*>(this_)->~T();
  }
  // ...
  ,                                           // skip-sample
  [](void* p, void const* other) {            // skip-sample
    new (p) T(*static_cast<T const*>(other)); // skip-sample
  },                                          // skip-sample
  sizeof(T)                                   // skip-sample
};
// end-sample

// sample(joined_vtable)
struct joined_vtable {
  vtable const* remote;
  void (*accelerate)(void* this_);
};

template <typename T>
joined_vtable const joined_vtable_for = {
  &vtable_for<T>,
  [](void* this_) {
    static_cast<T*>(this_)->accelerate();
  }
};
// end-sample

// sample(Vehicle)
struct Vehicle {
  template <typename Any>
  Vehicle(Any vehicle)
    : vtbl_{joined_vtable_for<Any>}
    , ptr_{std::malloc(sizeof(Any))}
  { new (ptr_) Any{vehicle}; }
                                                      // skip-sample
  Vehicle(Vehicle const& other)                       // skip-sample
    : vtbl_{other.vtbl_}                              // skip-sample
    , ptr_{std::malloc(other.vtbl_.remote->sizeof_)}  // skip-sample
  {                                                   // skip-sample
    other.vtbl_.remote->copy(ptr_, other.ptr_);       // skip-sample
  }                                                   // skip-sample

  void accelerate() {
    vtbl_.accelerate(ptr_);
  }

  ~Vehicle() {
    vtbl_.remote->dtor(ptr_);
    std::free(ptr_);
  }

private:
  joined_vtable const vtbl_;
  void* ptr_;
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
