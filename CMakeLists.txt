cmake_minimum_required(VERSION 3.5)
project(NB2016)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_path(Boost_cfg NAMES project-config.jam PATHS F:\\lib\\boost_1_61_0 D:\\lib\\boost_1_61_0)
set(Boost_INCLUDE_DIR ${Boost_cfg})

find_package(Boost COMPONENTS system thread log_setup log date_time filesystem chrono zlib iostreams)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

set(SOURCE_FILES main.cpp)
add_executable(NB2016 ${SOURCE_FILES})
target_link_libraries(NB2016 ${Boost_LIBRARIES} z)