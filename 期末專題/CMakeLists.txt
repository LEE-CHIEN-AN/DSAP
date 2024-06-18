cmake_minimum_required(VERSION 3.0.0)
project(DSAP112_FinalProject VERSION 0.1.0 LANGUAGES C CXX)

add_executable(GUI GUI.cpp)
target_compile_features(GUI PRIVATE cxx_std_17)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(GUI PRIVATE sfml-system sfml-network sfml-graphics sfml-window)

add_executable(PDOGS PDOGS.cpp)
target_compile_features(PDOGS PRIVATE cxx_std_17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})


include(CPack)
