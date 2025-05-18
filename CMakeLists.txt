cmake_minimum_required(VERSION 3.10)
project(monitoring)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/libs
    ${CMAKE_SOURCE_DIR}/metrics
    ${CMAKE_SOURCE_DIR}/outputs
)

file(GLOB METRICS_SRC "metrics/*.cpp")
file(GLOB OUTPUTS_SRC "outputs/*.cpp")

add_executable(monitor
    main.cpp
    ${METRICS_SRC}
    ${OUTPUTS_SRC}
)

target_link_libraries(monitor pthread)

install(TARGETS monitor RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
