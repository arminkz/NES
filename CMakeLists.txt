cmake_minimum_required(VERSION 3.20)

project(NES CXX)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)

set(LIBRARIES
    fmt::fmt
    spdlog::spdlog
    glfw
    GLEW::GLEW        
    glm::glm
    imgui::imgui
)

include_directories(src)

# Handle .cpp files (cmake recommends manually inputing each filename here) (i cant do that !)
file(GLOB SOURCES
    src/*
    src/utilities/*
    src/interface/fonts/*
    src/interface/imgui-impl/*
)

# Handle .h files
set(INCLUDES ${INCLUDES} src)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})
message(STATUS "I hate myself so I use cmake!")