// Copyright Louis Dionne 2017
// Distributed under the Boost Software License, Version 1.0.

#include <cstdlib>
#include <iostream>
#include <string>


struct vtable {
  void (*accelerate)(void* __this);
  void (*dtor)(void* __this);
};

template <typename T>
vtable const vtable_for{};

struct Vehicle {
  template <typename Any>
    // enabled only when vehicle.accelerate() is valid
  Vehicle(Any vehicle)
    : vptr_{&vtable_for<Any>}
  {
    impl_ = std::malloc(sizeof(Any));
    new (impl_) Any{vehicle};
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




struct Car {
  std::string make;
  int year;
  void accelerate() { std::cout << "Car::accelerate()" << std::endl; }
};

template <>
vtable const vtable_for<Car> = {
  /* accelerate = */ [](void* self) { static_cast<Car*>(self)->accelerate(); },
  /* ~Car = */       [](void* self) { static_cast<Car*>(self)->~Car(); }
};



void foo(Vehicle vehicle) {
  vehicle.accelerate();
}

int main() {
  Car audi{"Audi", 2017};
  foo(audi);
}
