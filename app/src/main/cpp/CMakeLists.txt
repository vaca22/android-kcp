cmake_minimum_required(VERSION 3.18.1)


project("myapplication")


add_library(myapplication
        SHARED
        native-lib.cpp
        ikcp.c)



find_library(
        log-lib
        log)

target_link_libraries(
        myapplication

        ${log-lib})