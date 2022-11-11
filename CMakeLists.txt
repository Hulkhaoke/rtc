cmake_minimum_required(VERSION 3.15)
project(ZTransport VERSION 0.1.0 LANGUAGES C CXX DESCRIPTION "A user-friendly data transport library")

##--------------------- Version file ---------------------------------------##
configure_file(src/version/version.h.in "${PROJECT_SOURCE_DIR}/src/c/version.h")

# Specified the language standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

##--------------------- Compile Options ------------------------------------##
# Configure compile options
#add_compile_options(-Wall -Wextra -Werror)

# Set build type
# set(CMAKE_BUILD_TYPE Debug)  # Use `cmake -DCMAKE_BUILD_TYPE=Debug` more better
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

##--------------------- Global Macros --------------------------------------##
add_definitions(-DZT_DLL_EXPORTS)

##--------------------- Include directories --------------------------------##
include_directories(src/c)

##--------------------- Source files ---------------------------------------##
file(GLOB_RECURSE ZT_LIB_SRC
    src/packet/*.cpp
)

add_library(zt STATIC ${ZT_LIB_SRC})

# Build demo executable
add_executable(demo test/main.cpp)
target_link_libraries(demo PRIVATE zt)

##--------------------- Install and Package target -------------------------##
# Install
INSTALL(TARGETS zt)
file(GLOB_RECURSE ZT_LIB_HEADERS src/packet/packet_api.h)
install(FILES ${ZT_LIB_HEADERS} DESTINATION include)
# 为dll设置linker
# 指定dll的生成目录，这里是：./lib_out
SET_TARGET_PROPERTIES(zt PROPERTIES LINKER_LANGUAGE C
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        OUTPUT_NAME "zt"
        PREFIX "")

#if (NOT USE_IMPORTED_LIB)
#    install(TARGETS demo zt
#            RUNTIME DESTINATION bin
#            LIBRARY DESTINATION lib
#            ARCHIVE DESTINATION lib
#            PUBLIC_HEADER DESTINATION include)
#    file(GLOB_RECURSE ZT_LIB_HEADERS src/transport/*.h)
#    install(FILES ${ZT_LIB_HEADERS} DESTINATION include)
#endif()