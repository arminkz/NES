cmake_minimum_required(VERSION 3.20)

project(NES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")


# Packages that are handled by conan
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)
find_package(glm REQUIRED)


include_directories(src)

# Packages that are git submodules
include_directories("${CMAKE_SOURCE_DIR}/external/imgui")
include_directories("${CMAKE_SOURCE_DIR}/external/stb")

add_subdirectory("${CMAKE_SOURCE_DIR}/external")
add_subdirectory("${CMAKE_SOURCE_DIR}/external/nativefiledialog-extended")
add_subdirectory("${CMAKE_SOURCE_DIR}/external/SDL2")
add_subdirectory("${CMAKE_SOURCE_DIR}/external/SDL_mixer")


# Libraries
set(LIBRARIES
    fmt::fmt
    spdlog::spdlog
    glfw
    GLEW::GLEW        
    glm::glm
    ImGui
    nfd
    SDL2-static
    SDL2_mixer
)


# SDL2 Mixer options
set(BUILD_SHARED_LIBS OFF)
set(SDL2MIXER_WAVPACK OFF)
set(SDL2MIXER_OPUS OFF)
set(SDL2MIXER_MOD OFF)
set(SDL2MIXER_MIDI OFF)


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