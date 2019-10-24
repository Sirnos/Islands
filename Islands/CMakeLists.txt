
cmake_minimum_required(VERSION 3.8.0)

project (Islands LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_RELEASE_LIBS       ON)
set(Boost_USE_MULTITHREADED      ON)

set(SFML_STATIC_LIBRARIES  ON)

find_package(SFML 2.5.1 COMPONENTS system window graphics REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(RapidXML REQUIRED)
find_package(Boost 1.70.0 REQUIRED COMPONENTS system filesystem)

file(GLOB SRC
		"${PROJECT_SOURCE_DIR}/FastNoise/*.cpp"
        "${PROJECT_SOURCE_DIR}/FastNoise/*.h"
        "${PROJECT_SOURCE_DIR}/*.hpp"
        "${PROJECT_SOURCE_DIR}/*.cpp"
        )

	
include_directories(${SFML_INCLUDE_DIR})
include_directories(${SQLite3_INCLUDE_DIR})
include_directories(${RAPIDXML_INCLUDE_DIRS})
include_directories(${BOOST_INCLUDE_DIRS})

add_executable(Islands ${SRC})

target_compile_features(Islands PRIVATE cxx_std_17)

target_link_libraries(Islands sfml-graphics sfml-window sfml-system ${SQLite3_LIBRARIES} Boost::boost Boost::system Boost::filesystem)

file(COPY Data DESTINATION ${CMAKE_BINARY_DIR})