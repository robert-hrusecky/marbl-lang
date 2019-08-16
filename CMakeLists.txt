cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(occ LANGUAGES CXX VERSION 0.0.1)

get_filename_component(CMAKE_MODULE_PATH cmake ABSOLUTE)

set(modules lexer parser)

# Setup testing
find_package(Catch2 REQUIRED)
include(CTest)
include(Catch)
add_executable(runtest src/runtest.cpp)
target_compile_options(runtest PRIVATE -Wall -Wextra)
target_link_libraries(runtest Catch2::Catch2)

include_directories(src)

# Add modules
foreach(module IN LISTS modules)
    add_subdirectory(src/${module})
endforeach()

add_executable(occ src/main.cpp)
target_compile_options(occ PRIVATE -Wall -Wextra)
target_link_libraries(occ ${modules})

# Discover test cases
set(testmodules "")
foreach(module ${modules})
    list(APPEND testmodules ${module}Test)
endforeach()
target_link_libraries(runtest -Wl,--whole-archive ${testmodules} -Wl,--no-whole-archive)
catch_discover_tests(runtest)
