cmake_minimum_required(VERSION 3.10)
project(RealTimeRankingAVL)

set(CMAKE_C_STANDARD 99)

add_executable(RealTimeRankingAVL 
    main.c 
    avl.c 
    avl.h 
    rn.c 
    rn.h 
    estruturas.h
)
