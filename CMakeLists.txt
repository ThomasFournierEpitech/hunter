project(myWord_v2)
cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -g3 -lsfml-graphics -lsfml-audio -lsfml-network -lsfml-system -lsfml‑window" CACHE STRING "c++ flags")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -m32 -g3" CACHE STRING "c flags")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

INCLUDE_DIRECTORIES(
${PROJECT_SOURCE_DIR}/include/
)

include_directories(${CMAKE_INCLUDE_PATH})
FILE(GLOB SRCS src/*.cpp)
FILE(GLOB HDRS include/*.hpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDAR_REQUIRED ON)
set(CMAKE CXX_EXTENSIONS OFF)

ADD_EXECUTABLE(hunter_network
   ${SRCS}
   ${HDRS}
)

target_link_libraries(hunter_network ${CONAN_LIBS})
