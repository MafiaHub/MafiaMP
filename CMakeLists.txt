cmake_minimum_required(VERSION 3.24...3.31)
cmake_policy(SET CMP0091 NEW)

# The Framework: the pinned prebuilt SDK (cmake/FrameworkPin.cmake), a local install tree
# (-DFW_SDK_ROOT) or a source checkout built alongside (-DFW_SOURCE_DIR). Must precede project():
# it selects the vcpkg toolchain.
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/FrameworkBootstrap.cmake)

project(MafiaMP CXX)
fw_bootstrap_finish()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

add_subdirectory(code)

# Deployment key for every RPC identifier, handed to the Framework at runtime through
# InstanceOptions::rpcIdentifierSalt. Release CI passes it per shipped build; client and server
# must carry the same value or no RPC dispatches (the build token gate catches the mismatch).
set(FW_RPC_IDENTIFIER_SALT "" CACHE STRING "64-bit salt keying every RPC identifier; must match on client and server")
# Release CI turns this on, so a salt that failed to arrive stops the configure instead of shipping a
# build keyed on the Framework's public default.
option(MAFIAMP_REQUIRE_RPC_IDENTIFIER_SALT "Fail the configure when FW_RPC_IDENTIFIER_SALT is unset" OFF)
if(MAFIAMP_REQUIRE_RPC_IDENTIFIER_SALT AND FW_RPC_IDENTIFIER_SALT STREQUAL "")
    message(FATAL_ERROR "MAFIAMP_REQUIRE_RPC_IDENTIFIER_SALT is ON but FW_RPC_IDENTIFIER_SALT is empty; refusing to build on the public default salt")
endif()
if(NOT FW_RPC_IDENTIFIER_SALT STREQUAL "")
    string(REGEX REPLACE "[uUlL]+$" "" _mafiamp_rpc_salt "${FW_RPC_IDENTIFIER_SALT}")
    if(NOT _mafiamp_rpc_salt MATCHES "^(0[xX][0-9a-fA-F]+|[0-9]+)$")
        message(FATAL_ERROR "FW_RPC_IDENTIFIER_SALT must be a hexadecimal (0x...) or decimal integer")
    endif()
    foreach(_mafiamp_target MafiaMPClient MafiaMPServer)
        if(TARGET ${_mafiamp_target})
            target_compile_definitions(${_mafiamp_target} PRIVATE MAFIAMP_RPC_IDENTIFIER_SALT=${_mafiamp_rpc_salt}ULL)
        endif()
    endforeach()
    message(STATUS "MafiaMP: RPC identifier salt set")
endif()
