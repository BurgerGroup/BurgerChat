cmake_minimum_required(VERSION 2.8)

project(BurgerChat C CXX)

enable_language(ASM)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(CXX_FLAGS
 -g
 # -DVALGRIND
 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 -Werror
 -Wconversion
 -Wno-unused-parameter
 -Wno-unused-but-set-variable
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 -std=c++11
 -rdynamic
 -pthread   # check
 )
if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  list(APPEND CXX_FLAGS "-Wno-null-dereference")
  list(APPEND CXX_FLAGS "-Wno-sign-conversion")
  list(APPEND CXX_FLAGS "-Wno-unused-local-typedef")
  list(APPEND CXX_FLAGS "-Wthread-safety")
  list(REMOVE_ITEM CXX_FLAGS "-rdynamic")
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")


# find_package(Curses REQUIRED)

# https://www.jianshu.com/p/f181b5bd0a63 这里需要更好的组织方式
include_directories(${CURSES_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/3rdparty)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/Burger)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/Burger/thirdparty)
include_directories(${PROJECT_SOURCE_DIR}/client)
include_directories(${PROJECT_SOURCE_DIR}/common)
include_directories(${PROJECT_SOURCE_DIR}/server)
include_directories(${PROJECT_SOURCE_DIR}/model)
include_directories(/usr/include)

link_directories(${PROJECT_SOURCE_DIR}/lib)

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/Burger/burger/base)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/Burger/burger/net)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/Burger/burger/db)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/Burger/thirdparty/googletest)
add_subdirectory(${PROJECT_SOURCE_DIR}/client)
add_subdirectory(${PROJECT_SOURCE_DIR}/server)
add_subdirectory(${PROJECT_SOURCE_DIR}/model)


