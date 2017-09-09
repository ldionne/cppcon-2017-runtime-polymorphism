// Copyright Louis Dionne 2017
// Distributed under the Boost Software License, Version 1.0.

#include <iostream>
#include <string>
#include <vector>


// sample(vtable)
struct vtable {
  void (*accelerate)(void* this_);
  void (*dtor)(void* this_);
};

template <typename T>
vtable const vtable_for = {
  [](void* this_) { static_cast<T*>(this_)->accelerate(); },
  [](void* this_) { static_cast<T*>(this_)->dtor(); }
};
// end-sample

// sample(Vehicle)
struct Vehicle {
  vtable const* vptr_;
  void accelerate() { vptr_->accelerate(this); }
  ~Vehicle() { vptr_->dtor(this); }
};
// end-sample

// sample(subclasses)
struct Car : Vehicle {
  Car() : Vehicle{&vtable_for<Car>} { }
  void accelerate();
  void dtor();
};

struct Truck : Vehicle {
  Truck() : Vehicle{&vtable_for<Truck>} { }
  void accelerate();
  void dtor();
};

struct Plane : Vehicle {
  Plane() : Vehicle{&vtable_for<Plane>} { }
  void accelerate();
  void dtor();
};
// end-sample

void Car::accelerate() { std::cout << "Car::accelerate()" << std::endl; }
void Car::dtor() { }

void Truck::accelerate() { std::cout << "Truck::accelerate()" << std::endl; }
void Truck::dtor() { }

void Plane::accelerate() { std::cout << "Plane::accelerate()" << std::endl; }
void Plane::dtor() { }

// sample(main)
int main() {
  std::vector<std::unique_ptr<Vehicle>> vehicles;

  vehicles.push_back(std::make_unique<Car>(/*...*/));
  vehicles.push_back(std::make_unique<Truck>(/*...*/));
  vehicles.push_back(std::make_unique<Plane>(/*...*/));

  for (auto& vehicle : vehicles) {
    vehicle->accelerate();
  }
}
// end-sample
