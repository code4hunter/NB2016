cmake_minimum_required(VERSION 3.5)
project(NB2016)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

#find boost
if(${WIN32})
find_path(Boost_cfg NAMES project-config.jam PATHS F:\\lib\\boost_1_61_0 D:\\lib\\boost_1_61_0)
set(Boost_INCLUDE_DIR ${Boost_cfg})
endif()

find_package(Boost COMPONENTS system thread log_setup log date_time filesystem chrono iostreams)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

#ctp
set(CTPROOT ${PROJECT_SOURCE_DIR}/doc/v6.3.6_tradeapi_20151215_2)
if(${WIN32})
    if(True)
        set(CTPPLATFORM tradeapi_windows64)
    else()
        set(CTPPLATFORM tradeapi_windows32)
    endif()
elseif(${UNIX})
    set(CTPPLATFORM tradeapi_linux64)
endif()
include_directories(${CTPROOT}/${CTPPLATFORM})
link_directories(${CTPROOT}/${CTPPLATFORM})

set(SOURCE_FILES main.cpp market_data.cpp market_data.h CTP_market_data_feeder.cpp CTP_market_data_feeder.h
        history_market_data_feeder.cpp history_market_data_feeder.h snapshot_manager.cpp snapshot_manager.h
        strategy_manager.cpp strategy_manager.h CTP_trade_svc.cpp CTP_trade_svc.h
        simulation_trade_svc.cpp simulation_trade_svc.h order_manager.cpp order_manager.h order.cpp order.h
        position_manager.cpp position_manager.h position.cpp position.h market_data_feeder.cpp market_data_feeder.h
        trade_svc.cpp trade_svc.h strategy_interface.cpp strategy_interface.h log.cpp log.h plugin_interface.h
        dynamic_library.cpp dynamic_library.h plugin_log.cpp plugin_log.h plugin_manager.cpp plugin_manager.h
        plugin_proxy.cpp plugin_proxy.h time_utility.cpp time_utility.h contract.cpp contract.h)
add_executable(NB2016 ${SOURCE_FILES})
target_link_libraries(NB2016 ${Boost_LIBRARIES} z thostmduserapi thosttraderapi)