cmake_minimum_required(VERSION 3.19)

project(qrparser)

#project(qrparser-rpi)
#set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
#set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
#set(CMAKE_LIBRARY_ARCHITECTURE aarch64-linux-gnu)

set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)
set(PATCH_VERSION 0)

message(STATUS "Configuring ${PROJECT_NAME} ${SH_MAJOR}.${SH_MINOR}.${SH_PATCH}")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17 -O3 -Wall -Wno-deprecated-declarations")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")  

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

add_definitions(-DFMT_HEADER_ONLY)

include_directories(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/third_party/nlio/include
        ${CMAKE_SOURCE_DIR}/third_party/nlassert/include
        ${CMAKE_SOURCE_DIR}/third_party/fmt/include
        )

set(SOURCE 
        ${CMAKE_SOURCE_DIR}/src/CHIPMem-Malloc.cpp
        ${CMAKE_SOURCE_DIR}/src/Base38Decode.cpp
        ${CMAKE_SOURCE_DIR}/src/BytesToHex.cpp
        ${CMAKE_SOURCE_DIR}/src/SetupPayload.cpp
        ${CMAKE_SOURCE_DIR}/src/QRCodeSetupPayloadParser.cpp
        ${CMAKE_SOURCE_DIR}/src/TLVReader.cpp
    )

add_library(${PROJECT_NAME}
        ${SOURCE}
        ${CMAKE_SOURCE_DIR}/src/interface.cpp
        )


add_executable(parser_test
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    )

target_link_libraries(parser_test PRIVATE ${PROJECT_NAME} 
    )

install(TARGETS ${PROJECT_NAME})
