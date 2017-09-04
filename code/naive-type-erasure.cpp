// Copyright Louis Dionne 2017
// Distributed under the Boost Software License, Version 1.0.

#include <iostream>
#include <memory>
#include <string>


struct VehicleBase {
  virtual std::unique_ptr<VehicleBase> clone() const = 0;
  virtual void accelerate_impl() = 0;
  virtual ~VehicleBase() { }
};

template <typename Any>
struct VehicleImpl : VehicleBase {
  VehicleImpl(Any vehicle)
    : vehicle_(vehicle)
  { }

  std::unique_ptr<VehicleBase> clone() const override
  { return std::make_unique<VehicleImpl>(vehicle_); }

  void accelerate_impl() override
  { vehicle_.accelerate(); }

  Any vehicle_;
};

struct Vehicle {
  template <typename Any>
    // enabled only when vehicle.accelerate() is valid
  Vehicle(Any vehicle)
    : impl_{std::make_unique<VehicleImpl<Any>>(vehicle)}
  { }

  Vehicle(Vehicle const& other)
    : impl_{other.impl_->clone()}
  { }

  void accelerate()
  { impl_->accelerate_impl(); }

private:
  std::unique_ptr<VehicleBase> impl_;
};


struct Car {
  std::string make;
  int year;
  void accelerate() { std::cout << "Car::accelerate()" << std::endl; }
};

void foo(Vehicle vehicle) {
  vehicle.accelerate();
}

int main() {
  Car audi{"Audi", 2017};
  foo(audi);
}
