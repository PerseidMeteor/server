cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "-std=c++17")
set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_CXX_FLAGS "-Wall")

project(server)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cc)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

link_directories(${CMAKE_CURRENT_LIST_DIR}/lib)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/server SERVER_SRC)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${SERVER_SRC}
)