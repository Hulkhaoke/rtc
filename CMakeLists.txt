cmake_minimum_required(VERSION 3.15)
project(ZTransport VERSION 0.1.0 LANGUAGES C CXX DESCRIPTION "A user-friendly data transport library")

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

##----------------------thirdparty----------------##
# 设置SDL2头文件与lib的目录，分别命名为SDL2_INCLUDE_DIR和SDL2_LIB_DIR供后续使用
set(SDL2_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/thirdparty/sdl2/include)
set(SDL2_LIB_DIR ${CMAKE_SOURCE_DIR}/src/thirdparty/sdl2/lib/windows/release/bin)
# 设置ffmpeg头文件与lib的目录
set(FFMPEG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/thirdparty/ffmpeg/include)
set(FFMPEG_LIB_DIR ${CMAKE_SOURCE_DIR}/src/thirdparty/ffmpeg/lib/release/bin)
# 设置libnice头文件与lib的目录
set(LIBNICE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/thirdparty/libnice/include)
set(LIBNICE_LIB_DIR ${CMAKE_SOURCE_DIR}/src/thirdparty/libnice/lib/windows/release/lib)

##--------------------- Include directories --------------------------------##
# 导入SDL2和FFMPEG的头文件
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${FFMPEG_INCLUDE_DIR})
include_directories(${LIBNICE_INCLUDE_DIR})

# 链接SDL2和FFMPEG到项目中
link_directories(${SDL2_LIB_DIR})
link_directories(${FFMPEG_LIB_DIR})
link_directories(${LIBNICE_LIB_DIR})

#include_directories(src/c)
##--------------------- Source files ---------------------------------------##
file(GLOB_RECURSE ZT_LIB_SRC
    src/packet/*.cpp
)

#add_library(zt STATIC ${ZT_LIB_SRC})

# Build demo executable
#add_executable(demo test/demo.cpp)
#add_executable(remotecontrol test/remotecontrol.cpp)
add_executable(nicetest test/nicetest.cpp)

#target_link_libraries(demo PRIVATE zt)
#target_link_libraries(remotecontrol SDL2 avcodec avdevice avfilter avformat avutil postproc swresample swscale)
target_link_libraries(nicetest PRIVATE nice)

# install(FILES "${FFMPEG_LIB_DIR}/avcodec-58.dll"
# 		DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
# 	)
# install(FILES "${FFMPEG_LIB_DIR}/avcodec-58.dll"
# 		DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
# 	)

# install(FILES "${FFMPEG_LIB_DIR}/avdevice-58.dll"
#     DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
# )
# install(FILES "${FFMPEG_LIB_DIR}/avdevice-58.dll"
#     DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
# )

# install(FILES "${FFMPEG_LIB_DIR}/avfilter-7.dll"
#     DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
# )
# install(FILES "${FFMPEG_LIB_DIR}/avfilter-7.dll"
#     DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
# )

# install(FILES "${FFMPEG_LIB_DIR}/avformat-58.dll"
#     DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
# )
# install(FILES "${FFMPEG_LIB_DIR}/avformat-58.dll"
#     DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
# )

# install(FILES "${FFMPEG_LIB_DIR}/avutil-56.dll"
#     DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
# )
# install(FILES "${FFMPEG_LIB_DIR}/avutil-56.dll"
#     DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
# )

# install(FILES "${FFMPEG_LIB_DIR}/postproc-55.dll"
#     DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
# )
# install(FILES "${FFMPEG_LIB_DIR}/postproc-55.dll"
#     DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
# )

# install(FILES "${FFMPEG_LIB_DIR}/swresample-3.dll"
#     DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
# )
# install(FILES "${FFMPEG_LIB_DIR}/swresample-3.dll"
#     DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
# )

# install(FILES "${FFMPEG_LIB_DIR}/swscale-5.dll"
#     DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
# )
# install(FILES "${FFMPEG_LIB_DIR}/swscale-5.dll"
#     DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
# )

# install(FILES "${SDL2_LIB_DIR}/SDL2.dll"
# 		DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
# 	)
# install(FILES "${SDL2_LIB_DIR}/SDL2.dll"
# 		DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
# 	)

install(FILES "${LIBNICE_LIB_DIR}/../bin/nice.dll"
		DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
	)
install(FILES "${LIBNICE_LIB_DIR}/../bin/nice.dll"
		DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
	)

##--------------------- Install and Package target -------------------------##
# Install
#INSTALL(TARGETS zt)
#file(GLOB_RECURSE ZT_LIB_HEADERS src/packet/packet_api.h)
#install(FILES ${ZT_LIB_HEADERS} DESTINATION include)
# 为dll设置linker
# 指定dll的生成目录，这里是：./lib_out
#SET_TARGET_PROPERTIES(zt PROPERTIES LINKER_LANGUAGE C
#        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
#        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
#        OUTPUT_NAME "zt"
#        PREFIX "")

#if (NOT USE_IMPORTED_LIB)
#    install(TARGETS demo zt
#            RUNTIME DESTINATION bin
#            LIBRARY DESTINATION lib
#            ARCHIVE DESTINATION lib
#            PUBLIC_HEADER DESTINATION include)
#    file(GLOB_RECURSE ZT_LIB_HEADERS src/transport/*.h)
#    install(FILES ${ZT_LIB_HEADERS} DESTINATION include)
#endif()