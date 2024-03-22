cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)

# download CPM.cmake
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

project(
  vml
  VERSION 0.0.1
  DESCRIPTION "Vector Math Library"
  LANGUAGES CXX)

add_library(vml INTERFACE)
target_include_directories(vml INTERFACE include)
target_sources(vml
  PRIVATE
    include/vml/arithmetic.hpp
    include/vml/common.hpp
    include/vml/complex.hpp
    include/vml/ext.hpp
    include/vml/fwd.hpp
    include/vml/intrin.hpp
    include/vml/matrix.hpp
    include/vml/quaternion.hpp
    include/vml/shapes.hpp
    include/vml/undef.hpp
    include/vml/vector.hpp
    include/vml/core.hpp
    include/vml/vml.hpp
)

if(NOT PROJECT_IS_TOP_LEVEL)
  return()
endif()

CPMAddPackage("gh:catchorg/Catch2@3.4.0")
add_executable(test)
target_include_directories(test
  PRIVATE
    test
    ${Catch2_SOURCE_DIR}/src
)
target_link_libraries(test PRIVATE vml Catch2::Catch2WithMain)
target_sources(test
  PRIVATE
    test/arithmetic.t.cpp
    test/base.t.cpp
    test/color.t.cpp
    test/complex.t.cpp
    test/ext.t.cpp
    test/matrix.t.cpp
    test/quaternion.t.cpp
    test/shapes.t.cpp
    test/vector.t.cpp
)
