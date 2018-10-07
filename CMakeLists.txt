cmake_minimum_required(VERSION 2.8.0)

SET(PROJECT_NAME "PointCloud")
project(${PROJECT_NAME})

include_directories(include)
file(GLOB_RECURSE SOURCES "src/*.cpp")

if(WIN32)
    include_directories(C:/glm)
    SET(GLFW3_ROOT C:/glfw-3.1.2 CACHE PATH "GLFW root")
    SET(GLEW_ROOT C:/glew-1.13.0 CACHE PATH "GLEW root")

SET(GLEW_USE_STATIC_LIBS ON)

ADD_DEFINITIONS( -DGLEW_STATIC )# -DSTB_IMAGE_IMPLEMENTATION -DSTBI_ONLY_TGA -DSTBI_SUPPORT_ZLIB)

add_executable(${PROJECT_NAME} ${SOURCES})

if(WIN32)
    SET(CMAKE_PREFIX_PATH ${GLEW_ROOT})
    SET(CMAKE_LIBRARY_PATH ${GLEW_ROOT}/lib)
endif()
find_package(GLEW REQUIRED)
if(GLEW_FOUND)
	include_directories(${GLEW_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARY})
endif(GLEW_FOUND)

if(WIN32)
    SET(CMAKE_PREFIX_PATH ${GLFW3_ROOT})
    SET(CMAKE_LIBRARY_PATH ${GLFW3_ROOT}/lib)
endif()
find_package(glfw3 REQUIRED)
if(glfw3_FOUND)
	target_link_libraries(${PROJECT_NAME} glfw)#${GLFW3_LIBRARY})
	include_directories(${GLFW3_INCLUDE_DIR})
endif(glfw3_FOUND)

find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY})
endif(OpenGL_FOUND)

SET(COMPILE_FLAGS "-m64 -std=c++14 -Wall -Wextra -pedantic")
if(WIN32)
    SET(LINK_FLAGS "-lgdi32")
endif()

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}" )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${LINK_FLAGS}" )
