cmake_minimum_required(VERSION 3.10)
project ( libuv_pipe_server_client )
include(ExternalProject)

ExternalProject_Add(
	libuv
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/libuv
	CMAKE_ARGS
		-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
      	-DANDROID_ABI=${ANDROID_ABI}
      	-DANDROID_NATIVE_API_LEVEL=${ANDROID_NATIVE_API_LEVEL}
		-DLIBUV_BUILD_TESTS=OFF
		-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/libuv/)

add_library(uv_static STATIC IMPORTED)
set_property(
    TARGET uv_static
    PROPERTY IMPORTED_LOCATION
            ${CMAKE_BINARY_DIR}/libuv/lib/libuv_a.a)

include_directories(${CMAKE_BINARY_DIR}/libuv/include)

list(APPEND uv_libraries
	uv_static)

#only supports for linux and android
if(NOT WIN32)
	if(NOT CMAKE_SYSTEM_NAME MATCHES "Android|OS390")
		list(APPEND uv_libraries
			pthread
			${CMAKE_DL_LIBS})
	endif()
endif()	

add_executable(uv_server ${CMAKE_SOURCE_DIR}/server_socket.cpp)
target_link_libraries(uv_server ${uv_libraries})
add_dependencies(uv_server libuv)

add_executable(uv_client ${CMAKE_SOURCE_DIR}/client_socket.cpp)
target_link_libraries(uv_client ${uv_libraries})
add_dependencies(uv_client libuv)
