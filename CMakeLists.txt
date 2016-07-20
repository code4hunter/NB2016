cmake_minimum_required(VERSION 3.5)
project(NB2016)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#find boost
find_path(Boost_cfg NAMES project-config.jam PATHS F:\\lib\\boost_1_61_0 D:\\lib\\boost_1_61_0)
set(Boost_INCLUDE_DIR ${Boost_cfg})

find_package(Boost COMPONENTS system thread log_setup log date_time filesystem chrono iostreams)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

#ctp
set(CTPROOT ${PROJECT_SOURCE_DIR}/doc/v6.3.6_tradeapi_20151215_2)
if(${WIN32})
    if(CMAKE_CL_64)
        set(CTPPLATFORM tradeapi_windows64)
    else()
        set(CTPPLATFORM tradeapi_windows32)
    endif()
elseif(${UNIX})
    set(CTPPLATFORM tradeapi_linux64)
endif()
include_directories(${CTPROOT}/${CTPPLATFORM})
link_directories(${CTPROOT}/${CTPPLATFORM})

set(SOURCE_FILES main.cpp)
add_executable(NB2016 ${SOURCE_FILES})
target_link_libraries(NB2016 ${Boost_LIBRARIES} z thostmduserapi thosttraderapi)