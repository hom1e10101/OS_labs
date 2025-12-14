cmake_minimum_required(VERSION 3.10)
project(os_lab LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/e
    ${CMAKE_SOURCE_DIR}/cos
)
add_library(contract1 SHARED
    e/e.cpp
    cos/cos.cpp
)
set_target_properties(contract1 PROPERTIES OUTPUT_NAME "contract1" PREFIX "")

add_library(contract2 SHARED
    e/e.cpp
    cos/cos.cpp
)
set_target_properties(contract2 PROPERTIES OUTPUT_NAME "contract2" PREFIX "")

add_executable(prog1 prog1/main.cpp)
target_link_libraries(prog1 PRIVATE contract1)

add_executable(prog2
    prog2/main.cpp
    prog2/DynamicLoader.cpp
)
target_link_libraries(prog2 PRIVATE dl)
add_custom_command(TARGET prog2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:contract2>
        $<TARGET_FILE_DIR:prog2>
)