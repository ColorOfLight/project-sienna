cmake_minimum_required(VERSION 3.14)
project(ProjectSienna VERSION 0.1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../web/public)

# Set headers and source files
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
file(GLOB_RECURSE HEADER_FILES "${INCLUDE_DIR}/*.h")
file(GLOB_RECURSE CPP_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Include directories
include_directories(${INCLUDE_DIR})

# Add the executable
add_executable(ProjectSienna ${HEADER_FILES} ${CPP_FILES})

# NOTE: Activate when using assets
# Copy assets to the build directory
# add_custom_target(copy_assets ALL
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
# )

# Set link flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CUSTOM_LINK_FLAGS "-fexceptions")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CUSTOM_LINK_FLAGS "-03")
endif()

set_target_properties(ProjectSienna PROPERTIES LINK_FLAGS "-sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sALLOW_MEMORY_GROWTH=1 -lembind ${CUSTOM_LINK_FLAGS}")
target_compile_definitions(ProjectSienna PRIVATE TARGET_EMSCRIPTEN)

# NOTE: Activate when using assets
# target_link_options(ProjectSienna PUBLIC --preload-file assets)

# Add third party libraries
add_subdirectory(third-party/glm-1.0.1)

target_link_libraries(ProjectSienna PRIVATE
  glm::glm)

target_include_directories(ProjectSienna PRIVATE
    third-party/glm-1.0.1/glm
)

