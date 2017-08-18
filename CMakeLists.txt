cmake_minimum_required(VERSION 3.9.0)
project(sdl2_tiled)
set(CMAKE_CXX_STANDARD 11)

set(SRC_DIR src)
set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/assets.cpp
    ${SRC_DIR}/level.cpp
)
set(HEADERS
    ${SRC_DIR}/assets.hpp
    ${SRC_DIR}/level.hpp
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

find_package(TMXLITE REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(
    ${TMXLITE_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${TMXLITE_LIBRARIES}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
)

