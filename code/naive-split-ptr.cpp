// Copyright Louis Dionne 2017
// Distributed under the Boost Software License, Version 1.0.

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>


struct vtable {
  void (*accelerate)(void* __this);
  void (*copy)(void* p, void const* other);
  void (*dtor)(void* __this);
  std::size_t sizeof_;
};

template <typename T>
vtable const vtable_for = {
  /* accelerate */ [](void* self) { static_cast<T*>(self)->accelerate(); },
  /* copy */       [](void* p, void const* other) { new (p) T(*static_cast<T const*>(other)); },
  /* ~T */         [](void* self) { static_cast<T*>(self)->~T(); },
  /* sizeof_ */    sizeof(T)
};

struct Vehicle {
  template <typename Any>
    // enabled only when vehicle.accelerate() is valid
  Vehicle(Any vehicle)
    : vptr_{&vtable_for<Any>}
    , impl_{std::malloc(sizeof(Any))}
  {
    new (impl_) Any{vehicle};
  }

  Vehicle(Vehicle const& other)
    : vptr_{other.vptr_}
    , impl_{std::malloc(other.vptr_->sizeof_)}
  {
    other.vptr_->copy(impl_, other.impl_);
  }

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
