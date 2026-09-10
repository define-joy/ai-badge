cmake_minimum_required(VERSION 3.16)
project(badge_host_tests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ROOT "${CMAKE_CURRENT_LIST_DIR}/../..")

add_executable(badge_host_tests
    test_runtime.cpp
    ${ROOT}/kit/src/AppRegistry.cpp
    ${ROOT}/kit/src/Badge.cpp
    ${ROOT}/core/PortHub.cpp
    ${ROOT}/core/Time.cpp
    ${ROOT}/core/EventPump.cpp
    ${ROOT}/core/PowerPolicy.cpp
    ${ROOT}/core/JobQueue.cpp
    ${ROOT}/core/Runtime.cpp
    ${ROOT}/adapters/board/bringup.cpp
)

target_compile_definitions(badge_host_tests PRIVATE BADGE_HOST=1)
target_include_directories(badge_host_tests PRIVATE
    ${ROOT}/kit/include
    ${ROOT}
)

if(MSVC)
    target_compile_options(badge_host_tests PRIVATE /EHs-c- /GR-)
else()
    target_compile_options(badge_host_tests PRIVATE -fno-exceptions -fno-rtti)
endif()
