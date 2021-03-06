cmake_minimum_required (VERSION 2.8.2)

project(FunctionDraw)

# Find Boost lib
set(BOOST_ROOT "" CACHE PATH "Where to look for Boost")
find_package(Boost 1.50.0 REQUIRED)

if(${Boost_VERSION} LESS 105600)
  add_definitions(-DBOOST_SPIRIT_USE_PHOENIX_V3)
endif()

mark_as_advanced(CLEAR BOOST_ROOT)
if(${CMAKE_VERSION} VERSION_LESS "3.1.0")
  add_compile_options(-std=c++11)
else()
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()
include_directories(${Boost_INCLUDE_DIR})

# Add function drawing library
add_subdirectory(lib)

# Unit tests
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
  add_subdirectory(test)
endif()

# Expression to SVG drawing app
option(BUILD_EXP2SVG "Build expression to SVG drawing app" ON)
if(BUILD_EXP2SVG)
  add_subdirectory(exp2svg)
endif()
