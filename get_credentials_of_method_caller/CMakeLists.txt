cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

project(
  DBusGetCredentialsExample
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
  Qt5 REQUIRED
  COMPONENTS DBus)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(
  PkgConfig REQUIRED)
pkg_check_modules(Systemd REQUIRED IMPORTED_TARGET libsystemd)

include(./cmake/PFL.cmake)

pfl_init()

if(PROEJCTTEMPLATE_ENABLE_TESTING)
  enable_testing()
endif()

set(SERVER_DBUS_SERVICE_NAME org.example.${PROJECT_NAME}Server)
set(SERVER_DBUS_OBJECT_PATH /org/example/${PROJECT_NAME}Server)
set(SERVER_DBUS_INTERFACE_NAME ${SERVER_DBUS_SERVICE_NAME})

pfl_add_library(
  DISABLE_INSTALL
  MERGED_HEADER_PLACEMENT
  VERSION
  ${PROJECT_VERSION}
  SOURCES
  ./src/${PROJECT_NAME}/configure.hpp.in
  ./src/${PROJECT_NAME}/Server.hpp
  ./src/${PROJECT_NAME}/Server.cpp
  ./src/${PROJECT_NAME}/ServerSDBus.hpp
  ./src/${PROJECT_NAME}/ServerSDBus.cpp
  ./src/${PROJECT_NAME}/init.hpp
  ./src/${PROJECT_NAME}/init.cpp
  APPS
  server-sd_bus
  server
  client
  COMPILE_FEATURES
  PUBLIC
  cxx_std_17
  LINK_LIBRARIES
  PUBLIC
  Qt::DBus
  tl::expected
  PkgConfig::Systemd)

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
  ${PROJECT_NAME}::${PROJECT_NAME}
  ${PROJECT_SOURCE_DIR}/api/dbus/${SERVER_DBUS_INTERFACE_NAME}.xml
  ${PROJECT_NAME}/Server.hpp
  ${PROJECT_NAME}::Server
  src/${PROJECT_NAME}/ServerAdaptor
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
  ${PROJECT_NAME}::${PROJECT_NAME}
  ${PROJECT_SOURCE_DIR}/api/dbus/${SERVER_DBUS_INTERFACE_NAME}.xml
  ./src/${PROJECT_NAME}/ServerInterface)
