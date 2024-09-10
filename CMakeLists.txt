cmake_minimum_required(VERSION 3.26)
project(fusion)

set(CMAKE_CXX_STANDARD 20)

include_directories(fusion)

add_executable(fusion
        fusion/director.h
        fusion/event.h
        fusion/isolation.h
        fusion/main.cpp
        fusion/observer.h
        fusion/system.h
        fusion/unit_population.h
        fusion/niche.h)
