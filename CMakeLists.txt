cmake_minimum_required(VERSION 3.20)

project(NES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)
find_package(glm REQUIRED)
# find_package(imgui REQUIRED)

set(LIBRARIES
    fmt::fmt
    spdlog::spdlog
    glfw
    GLEW::GLEW        
    glm::glm
    ImGui
    nfd
)

include_directories(src)
include_directories("${CMAKE_SOURCE_DIR}/external/imgui")
include_directories("${CMAKE_SOURCE_DIR}/external/stb")

add_subdirectory("${CMAKE_SOURCE_DIR}/external")
add_subdirectory("${CMAKE_SOURCE_DIR}/external/nativefiledialog-extended")

# Handle .cpp files (cmake recommends manually inputing each filename here) (i cant do that !)
file(GLOB SOURCES
    src/*
    src/mappers/*
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


# Copy fonts
add_custom_command(TARGET ${PROJECT_NAME} 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/fonts/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/fonts)