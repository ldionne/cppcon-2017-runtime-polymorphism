// Copyright Louis Dionne 2017
// Distributed under the Boost Software License, Version 1.0.

#include <iostream>
#include <memory>
#include <string>
#include <vector>


// sample(VehicleBase)
struct VehicleBase {
  virtual std::unique_ptr<VehicleBase> clone() const = 0;
  virtual void accelerate() = 0;
  virtual ~VehicleBase() { }
};

template <typename Any>
struct VehicleImpl : VehicleBase {
  VehicleImpl(Any vehicle)
    : vehicle_(vehicle)
  { }

  std::unique_ptr<VehicleBase> clone() const override
  { return std::make_unique<VehicleImpl>(vehicle_); }

  void accelerate() override
  { vehicle_.accelerate(); }

  Any vehicle_;
};
// end-sample

// sample(Vehicle)
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
  { impl_->accelerate(); }

private:
  std::unique_ptr<VehicleBase> impl_;
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
