cmake_minimum_required(VERSION 3.30)
project(untitled C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled main.c
        globals.h
        algorithm_structs_PUBLIC/MapPoint.h
        globals.c
        algorithm_structs_PUBLIC/Path.h
        algorithm_structs_PUBLIC/FundamentalPath.h
        algorithm_structs_PUBLIC/FundamentalPath.c
        algorithm_structs_PUBLIC/MapPoint.c
        algorithm_structs_PUBLIC/Path.c
        track_files_PRIVATE/track_generation.c
        track_files_PRIVATE/track_generation.h
        track_files_PRIVATE/track_navigation.c
        track_files_PRIVATE/track_navigation.h
        exploration.c
        exploration.h
        track_files_PRIVATE/track_detection.h
        track_files_PRIVATE/track_detection.c
        direction.h
        direction.c
        navigate.c
        navigate.h)
