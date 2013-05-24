cmake_minimum_required(VERSION 2.8)

project( optical_flow_sta )

find_package( OpenCV REQUIRED )

#find_package(OpenMP)
#if (OPENMP_FOUND)
#	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#endif()

file( GLOB optical_flow_sta_SRC "src/*.h" "src/*.cpp" )

add_executable( optical_flow_sta ${optical_flow_sta_SRC} )
target_link_libraries( optical_flow_sta ${OpenCV_LIBS} )

set(CMAKE_BUILD_TYPE Release)