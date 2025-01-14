cmake_minimum_required(VERSION 3.30)
project(MemorySystemSim C)

set(CMAKE_C_STANDARD 17)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")

add_executable(MemorySystemSim main.c
        src/cache.c
        include/cache.h
        src/main_memory.c
        include/main_memory.h
        src/mmu.c
        include/definition.h
        src/register.c
        include/register.h
        include/mmu.h
)
