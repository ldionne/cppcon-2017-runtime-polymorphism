// Copyright Louis Dionne 2017
// Distributed under the Boost Software License, Version 1.0.

#include "vtable.hpp"

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>


// sample(Vehicle::members)
template <std::size_t N>
struct Vehicle {
private:
  vtable const* vptr_;
  union {
    void* ptr_;
    std::aligned_storage_t<N> buffer_;
  };
  bool on_heap_;
// end-sample

public:
// sample(Vehicle::Vehicle)
  template <typename Any>
    // enabled only when vehicle.accelerate() is valid
  Vehicle(Any vehicle)
    : vptr_{&vtable_for<Any>}
  {
    if (sizeof(Any) > N) {
      on_heap_ = true;
      ptr_ = std::malloc(sizeof(Any));
      new (ptr_) Any{vehicle};
    } else {
      on_heap_ = false;
      new (&buffer_) Any{vehicle};
    }
  }
// end-sample

  Vehicle(Vehicle const& other)
    : vptr_{other.vptr_}
  {
    if (other.on_heap_) {
      ptr_ = std::malloc(other.vptr_->sizeof_);
      other.vptr_->copy(ptr_, other.ptr_);
    } else {
      other.vptr_->copy(&buffer_, &other.buffer_);
    }
  }

  void accelerate() {
    vptr_->accelerate(on_heap_ ? ptr_ : &buffer_);
  }

  ~Vehicle() {
    if (on_heap_) {
      vptr_->dtor(ptr_);
      std::free(ptr_);
    } else {
      vptr_->dtor(&buffer_);
    }
  }
};



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
  std::vector<Vehicle<16>> vehicles;

  vehicles.push_back(Car{"Audi", 2017});
  vehicles.push_back(Truck{"Chevrolet", 2015});
  vehicles.push_back(Plane{"Boeing", "747"});

  for (auto& vehicle : vehicles) {
    vehicle.accelerate();
  }
}
// end-sample
