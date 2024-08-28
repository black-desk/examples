cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

project(
  QDBusGetCredentialsExample
  VERSION 0.1.0
  DESCRIPTION "An example project."
  HOMEPAGE_URL
    "https://github.com/black-desk/dbus-examples/blob/master/get_credentials_of_method_caller"
  LANGUAGES CXX)

include(./cmake/GitSemver.cmake)

gitsemver(PROJECT_VERSION)

include(./cmake/CPM.cmake)

CPMFindPackage(
  NAME expected
  VERSION 1.0.0
  GITHUB_REPOSITORY TartanLlama/expected
  GIT_TAG v1.0.0
  GIT_SHALLOW ON
  OPTIONS "EXPECTED_BUILD_TESTS OFF"
  FIND_PACKAGE_ARGUMENTS "NAMES tl-expected"
  EXCLUDE_FROM_ALL ON)

find_package(
  Qt5
  COMPONENTS DBus)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include(./cmake/PFL.cmake)

pfl_init()

if(PROEJCTTEMPLATE_ENABLE_TESTING)
  enable_testing()
endif()

pfl_add_library(
  DISABLE_INSTALL
  MERGED_HEADER_PLACEMENT
  VERSION
  ${PROJECT_VERSION}
  SOURCES
  ./src/QDBusGetCredentialsExample/configure.hpp.in
  ./src/QDBusGetCredentialsExample/Server.hpp
  ./src/QDBusGetCredentialsExample/Server.cpp
  ./src/QDBusGetCredentialsExample/init.hpp
  ./src/QDBusGetCredentialsExample/init.cpp
  APPS
  server
  client
  COMPILE_FEATURES
  PUBLIC
  cxx_std_17
  LINK_LIBRARIES
  PUBLIC
  Qt::DBus
  tl::expected)

function(get_real_target_name output target)
  get_target_property("${output}" "${target}" ALIASED_TARGET)
  if("${output}" STREQUAL "")
    set("${output}" "${target}")
  endif()
  set("${output}"
      ${${output}}
      PARENT_SCOPE)
endfunction()

function(
  qtdbusp2pexample_add_dbus_adaptor
  target_name
  xml
  includes
  parent_class
  basename
  classname)
  set(DBUS_ADAPTOR_SOURCE)

  qt_add_dbus_adaptor(DBUS_ADAPTOR_SOURCE ${xml} ${includes} ${parent_class}
                      ${basename} ${classname})
  # FIXME(black_desk): `qt_add_dbus_adaptor` will use basename as include and
  # this cannot be configured at all. This is just a workaround.
  get_real_target_name(target_name ${target_name})
  target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
  target_sources(${target_name} PRIVATE ${DBUS_ADAPTOR_SOURCE})
endfunction()

qtdbusp2pexample_add_dbus_adaptor(
  QDBusGetCredentialsExample::QDBusGetCredentialsExample
  ${PROJECT_SOURCE_DIR}/api/dbus/io.github.blackdesk.QDBusGetCredentialsExample.Server.xml
  QDBusGetCredentialsExample/Server.hpp
  QDBusGetCredentialsExample::Server
  src/QDBusGetCredentialsExample/ServerAdaptor
  ServerAdaptor)

function(qtdbusp2pexample_add_dbus_interface target xml basename) # include
  set_source_files_properties(${xml} NO_NAMESPACE ON)

  set(include "${ARGV3}")
  if(include)
    set_source_files_properties(${xml} PROPERTIES INCLUDE ${include})
  endif()

  set(INTERFACE_SOURCES)
  set(Qt5DBus_QDBUSXML2CPP_EXECUTABLE qdbusxml2cpp)
  qt_add_dbus_interface(INTERFACE_SOURCES ${xml} ${basename})
  get_real_target_name(target ${target})
  target_sources(${target} PRIVATE ${INTERFACE_SOURCES})
endfunction()

qtdbusp2pexample_add_dbus_interface(
  QDBusGetCredentialsExample::QDBusGetCredentialsExample
  ${PROJECT_SOURCE_DIR}/api/dbus/io.github.blackdesk.QDBusGetCredentialsExample.Server.xml
  ./src/QDBusGetCredentialsExample/io_github_blackdesk_QDBusGetCredentialsExample_Server)
