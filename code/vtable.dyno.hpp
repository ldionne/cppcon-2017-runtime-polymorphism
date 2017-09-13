// Copyright Louis Dionne 2017
// Distributed under the Boost Software License, Version 1.0.

#ifndef VTABLE_DYNO_HPP
#define VTABLE_DYNO_HPP

#include <dyno.hpp>
using namespace dyno::literals;


struct Vehicle : decltype(dyno::requires(
  dyno::CopyConstructible{},
  "accelerate"_s = dyno::function<void(dyno::T&)>
)) { };

template <typename T>
auto const dyno::default_concept_map<Vehicle, T> = dyno::make_concept_map(
  "accelerate"_s = [](T& vehicle) { vehicle.accelerate(); }
);

#endif // header guard
