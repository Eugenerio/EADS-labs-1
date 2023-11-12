cmake_minimum_required(VERSION 3.25)
project(EADS-labs-1)



option(FETCHCONTENT_QUIET "" OFF)
option(FETCHCONTENT_UPDATES_DISCONNECTED "" ON)
Include(FetchContent)

FetchContent_Declare(Catch2
        URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.4.0.tar.gz
        )

FetchContent_MakeAvailable(Catch2)


set(CMAKE_CXX_STANDARD 17)

add_executable(EADS_labs-1 main.cpp sequence.hpp split.hpp)
target_link_libraries(EADS_labs-1 PRIVATE Catch2::Catch2WithMain)

