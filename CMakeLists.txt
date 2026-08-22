cmake_minimum_required(VERSION 3.23)
project(termina)
#find_package(raylib REQUIRED)

add_executable(r)
#target_link_libraries(r PRIVATE raylib)
target_sources(r
    PRIVATE
    #src/main.c
    src/main.c
)
