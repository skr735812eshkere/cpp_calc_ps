cmake_minimum_required(VERSION 3.15)
project(homework2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(WIN32)
    set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "")
    
    add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    
    if(MSVC)
        add_compile_options(/W4 /permissive- /EHsc)
    endif()
    
    if(MINGW)
        add_compile_options(-Wall -Wextra -Wpedantic)
    endif()
endif()

if(APPLE)
    add_compile_options(-Wall -Wextra -Wpedantic)
    set(CMAKE_MACOSX_RPATH ON)
    set(CMAKE_INSTALL_RPATH "@executable_path")
endif()

include_directories(headers)

set(SOURCES
    source/subject.cpp
    source/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

file(COPY headers/ DESTINATION ${CMAKE_BINARY_DIR}/headers)

if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_RPATH ON
        INSTALL_RPATH "@executable_path"
    )
endif()
