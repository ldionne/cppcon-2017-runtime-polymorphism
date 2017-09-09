// Copyright Louis Dionne 2017
// Distributed under the Boost Software License, Version 1.0.

#include "vtable.hpp"

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>


// sample(Vehicle)
struct Vehicle {
  template <typename Any>
  Vehicle(Any vehicle)
    : vtbl_{vtable_for<Any>}
    , ptr_{std::malloc(sizeof(Any))}
  { new (ptr_) Any{vehicle}; }
                                                      // skip-sample
  Vehicle(Vehicle const& other)                       // skip-sample
    : vtbl_{other.vtbl_}                              // skip-sample
    , ptr_{std::malloc(other.vtbl_.sizeof_)}          // skip-sample
  {                                                   // skip-sample
    other.vtbl_.copy(ptr_, other.ptr_);               // skip-sample
  }                                                   // skip-sample

  void accelerate() {
    vtbl_.accelerate(ptr_);
  }

  ~Vehicle() {
    vtbl_.dtor(ptr_);
    std::free(ptr_);
  }

private:
  vtable const vtbl_; // <= not a pointer!
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
