# Copyright Louis Dionne 2017
# Distributed under the Boost Software License, Version 1.0.

cmake_minimum_required(VERSION 3.8)

add_executable(naive code/naive.cpp)
target_compile_features(naive PRIVATE cxx_std_14)

add_executable(naive-split-ptr code/naive-split-ptr.cpp)
target_compile_features(naive-split-ptr PRIVATE cxx_std_14)

add_executable(sbo-split-ptr code/sbo-split-ptr.cpp)
target_compile_features(sbo-split-ptr PRIVATE cxx_std_14)
