#
# Project Properties
#

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()
CMAKE_MINIMUM_REQUIRED (VERSION 3.10 FATAL_ERROR)
PROJECT (redisgraphapi LANGUAGES CXX)
SET (APPLICATION_NAME "RedisGraph C++ API")
SET (APPLICATION_CODENAME "${PROJECT_NAME}")
SET (APPLICATION_COPYRIGHT_YEARS "2019")
SET (APPLICATION_VERSION_MAJOR 1)
SET (APPLICATION_VERSION_MINOR 0)
SET (APPLICATION_VERSION_PATCH 0)
SET (APPLICATION_VERSION_TYPE SNAPSHOT)
SET (APPLICATION_VERSION_STRING "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}-${APPLICATION_VERSION_TYPE}")
SET (APPLICATION_ID "${APPLICATION_VENDOR_ID}.${PROJECT_NAME}")
SET (DEMO_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/demo")
#
# Compiler Options
#
SET (CMAKE_VERBOSE_MAKEFILE 0) # Use 1 for debugging, 0 for release
SET (CMAKE_CXX_STANDARD 17)
SET (CMAKE_CXX_STANDARD_REQUIRED ON)

# Cmake find modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")
find_package(GRPC  REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(c-ares CONFIG REQUIRED)

#
# Project Output Paths
#
SET (MAINFOLDER ${PROJECT_SOURCE_DIR})
find_package(Boost REQUIRED)

#
# Project Search Paths
#
LIST (APPEND CMAKE_PREFIX_PATH "${MAINFOLDER}")
include_directories("${MAINFOLDER}/include" "${MAINFOLDER}/third-party/cpp-bredis/include" ${Boost_INCLUDE_DIRS})
#
# Locate Project Prerequisites
#
#
# Add Build Targets
#
include (CTest)
enable_testing()
add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)
add_definitions(-DBOOST_COROUTINES_NO_DEPRECATION_WARNING)

include_directories(${Protobuf_INCLUDE_DIRS})
set(PROTOS
    ${CMAKE_CURRENT_SOURCE_DIR}/fraud_service.proto
)
set(PROTO_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proto-generated)
set(PROTO_SRC_PYTHON_DIR ${CMAKE_CURRENT_BINARY_DIR}/protopy-generated)

file(MAKE_DIRECTORY ${PROTO_SRC_DIR})
file(MAKE_DIRECTORY ${PROTO_SRC_PYTHON_DIR})


include_directories(${PROTO_SRC_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${SERVER_INCLUDE})
include_directories(SYSTEM ${OPENSSL_INCLUDE_DIR})

#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_SRC_DIR} ${PROTOS})
grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTOS})
grpc_generate_python(GRPC_PY ${PROTOS})

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")

    execute_process(COMMAND
        ${CMAKE_CXX_COMPILER} -dumpversion
        OUTPUT_VARIABLE GCC_VERSION)
    string(STRIP ${GCC_VERSION} GCC_VERSION)
    MESSAGE(STATUS "gcc version: [" ${GCC_VERSION} "]")

    # this works on gentoo, but not on travis/ubuntu, let's hard-code the value
    #execute_process(COMMAND
    #    "which" "gcov-${GCC_VERSION}"
    #    OUTPUT_VARIABLE GCOV_TOOL)
    #MESSAGE(STATUS "gcov: [" ${GCOV_TOOL} "]")
    #string(STRIP ${GCOV_TOOL} GCOV_TOOL)
    #MESSAGE(STATUS "gcov(fixed): [" ${GCOV_TOOL} "]")
    set(GCOV_TOOL "gcov-5")

    add_custom_target("coverage"
        COMMAND "lcov" --directory . --zerocounters
        COMMAND ctest
        COMMAND "lcov" --directory . --capture --output-file coverage.info --gcov-tool ${GCOV_TOOL}
        COMMAND "lcov" --remove coverage.info "'/t/*'" "'/boost_1_63_0/*'" "'/usr/*'" --output-file coverage.info.cleaned
        COMMAND "rm" coverage.info

        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    add_custom_target("coverage-report"
        COMMAND "genhtml" -o coverage coverage.info.cleaned
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS "coverage"
    )
endif()

MESSAGE(STATUS "boost inc: " ${Boost_INCLUDE_DIRS})


MESSAGE(STATUS "boost libs: " ${Boost_LIBRARIES})
add_library(catch_lib ${MAINFOLDER}/test/tests-main.cpp)
SET(GRPC_LIB "C:/Users/giorg/vcpkg1/vcpkg/installed/x64-windows/debug/lib/cares.lib;C:/Users/giorg/vcpkg1/vcpkg/installed/x64-windows/debug/lib/address_sorting.lib;C:/Users/giorg/vcpkg1/vcpkg/installed/x64-windows/debug/lib/gpr.lib;C:/Users/giorg/vcpkg1/vcpkg/installed/x64-windows/debug/lib/grpc.lib;C:/Users/giorg/vcpkg1/vcpkg/installed/x64-windows/debug/lib/grpc++.lib;C:/Users/giorg/vcpkg1/vcpkg/installed/x64-windows/debug/lib/grpc++_unsecure.lib")

if(WIN32)
# we support just windows 10
    set (USE_WINSOCK "ws2_32.lib")
    set(Boost_USE_MULTITHREADED ON)
    set(LINK_DEPENDENCIES ${Boost_LIBRARIES} catch_lib)
    add_definitions(-DBOOST_ALL_DYN_LINK -DBOOST_THREAD_USE_LIB -D_WIN32_WINNT=0x0A00)
else()
    #add_definitions(-Wall -Wextra -pedantic -Werror)
    add_definitions(-Wall -Wextra -pedantic)
    #add_definitions(-fsanitize=undefined -fsanitize=address -fno-omit-frame-pointer)
    set(LINK_DEPENDENCIES pthread ${Boost_LIBRARIES} catch_lib) #  asan ubsan
endif()

if ((Boost_MAJOR_VERSION GREATER_EQUAL 1) AND (Boost_MINOR_VERSION GREATER_EQUAL 70))
    set(BOOST_VERSION_OK true)
else()
    set(BOOST_VERSION_OK false)
endif()

MESSAGE(STATUS "BOOST_VERSION_OK ${BOOST_VERSION_OK}")

add_executable(t-01-initgraph test/01-initgraph.cpp)
target_link_libraries(t-01-initgraph ${LINK_DEPENDENCIES})
add_test("Should_Init_GraphCorrectly" t-01-initgraph)

add_executable(t-02-edgegraph test/02-edgegraph.cpp)
target_link_libraries(t-02-edgegraph ${LINK_DEPENDENCIES})
add_test("Should_Work_EdgeCorrectly" t-02-edgegraph)

add_executable(t-03-redisconnection test/03-redisconnection.cpp)
target_link_libraries(t-03-redisconnection ${LINK_DEPENDENCIES})
add_test("Should_Connect_RedisCorrectly" t-03-redisconnection)

add_executable(t-04-redisquerygraph test/04-redisquerygraph.cpp)
target_link_libraries(t-04-redisquerygraph ${LINK_DEPENDENCIES})
add_test("Should_Query_RedisGraphCorrectly" t-04-redisquerygraph)

set(FDS_SOURCES demo/main.cpp)

add_executable(fds ${GRPC_SRCS} ${FDS_SOURCES})
message(STATUS "GRPC libs : ${GRPC_LIBRARIES}")
target_link_libraries(fds PRIVATE 
                      ${OPENSSL_LIBRARIES}
                      ${ZLIB_LIBRARIES}
                      ${CMAKE_DL_LIBS} 
                      ${Boost_LIBRARIES}
                      ${c-ares_LIBRARIES}
                      ${USE_WINSOCK}
                      ${GRPC_LIB}
                      ${PROTOBUF_LIBRARIES}
                      )