cmake_minimum_required(VERSION 3.11.4) # for RHEL 8

# Let's place extra cmake scripts in /cmake directory
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Use this command to update PFL.cmake from GitHub release page:
#[[
wget -O cmake/PFL.cmake https://github.com/black-desk/PFL.cmake/releases/latest/download/PFL.cmake
]]
include(PFL)

project(
  use_std_expected
  VERSION 0.1.0
  DESCRIPTION "Examples of using std::expected"
  HOMEPAGE_URL "https://github.com/black-desk/examples/C++/use_std_expected"
  LANGUAGES CXX)

# Use this command to update GitSemver.cmake from GitHub release page:
#[[
wget -O cmake/GitSemver.cmake https://github.com/black-desk/GitSemver.cmake/releases/latest/download/GitSemver.cmake
]]
include(GitSemver)
gitsemver(PROJECT_VERSION)

pfl_init()

pfl_add_executable(
  DISABLE_INSTALL
  OUTPUT_NAME
  use_std_expected
  SOURCES
  src/main.cpp
  PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)
