cmake_minimum_required(VERSION 3.29)
project(RealTimeRankingAVL C)

set(CMAKE_C_STANDARD 11)

add_executable(RealTimeRankingAVL main.c avl.c rn.c sistema_ranking.c desempenho.c)
