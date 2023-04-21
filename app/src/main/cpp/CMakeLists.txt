cmake_minimum_required(VERSION 3.18.1)


project("myapplication")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/juice)

set(LIBJUICE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/get_name.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/addr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/agent.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/crc32.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/const_time.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/conn.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/conn_poll.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/conn_thread.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/conn_mux.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/base64.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/hash.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/hmac.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/ice.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/juice.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/log.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/random.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/server.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/stun.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/timestamp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/turn.c
        ${CMAKE_CURRENT_SOURCE_DIR}/juice/udp.c
        )

add_library(myapplication
        SHARED
        native-lib.cpp
        ikcp.c ${LIBJUICE_SOURCES})



find_library(
        log-lib
        log)

target_link_libraries(
        myapplication

        ${log-lib})