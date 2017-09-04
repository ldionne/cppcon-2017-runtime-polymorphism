# Copyright Louis Dionne 2017
# Distributed under the Boost Software License, Version 1.0.

cmake_minimum_required(VERSION 3.8)

add_executable(naive-type-erasure code/naive-type-erasure.cpp)
target_compile_features(naive-type-erasure PRIVATE cxx_std_14)

add_executable(naive-type-erasure-split-ptr code/naive-type-erasure-split-ptr.cpp)
target_compile_features(naive-type-erasure-split-ptr PRIVATE cxx_std_14)
