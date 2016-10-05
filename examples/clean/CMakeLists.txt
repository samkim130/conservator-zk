cmake_minimum_required(VERSION 2.8)
project(conservator_clean)

file(GLOB CLEAN_SRC clean_example.cpp)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

find_package(Threads REQUIRED)
find_package(Zookeeper REQUIRED)
find_package(Check REQUIRED)
find_package(Conservator REQUIRED)
#enable_testing()

include_directories(${CHECK_INCLUDE_DIRS})
include_directories(${ZOOKEEPER_INCLUDE_DIR}/zookeeper)
include_directories(${CONSERVATOR_INCLUDE_DIR}/conservator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(clean_example ${CLEAN_SRC})

target_link_libraries(clean_example ${ZOOKEEPER_LIBRARIES} ${CONSERVATOR_LIBRARIES})

