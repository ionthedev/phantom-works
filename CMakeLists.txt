# PhantomWorks/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(PhantomWorks)

set(CMAKE_CXX_STANDARD 17)
set(PHANTOM_ENGINE_DIR ../phantom-engine)
# Add PhantomEngine as an external project or a subdirectory
if(!TOP_LEVEL)
    add_subdirectory(${PHANTOM_ENGINE_DIR} ${CMAKE_BINARY_DIR}/phantom-engine)
    add_subdirectory(${PHANTOM_ENGINE_DIR} ${CMAKE_BINARY_DIR}/phantom-engine/src/ThirdParty/imgui-docking)
    add_subdirectory(${PHANTOM_ENGINE_DIR} ${CMAKE_BINARY_DIR}/phantom-engine/src/ThirdParty/rlImGui)
endif ()


# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)


# Add the PhantomWorks executable

add_executable(PhantomWorks src/PhantomWorks.cpp)



# Link against PhantomEngine
target_link_libraries(PhantomWorks PRIVATE PhantomEngine)

# Include directories
target_include_directories(PhantomWorks
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../phantom-engine/src
)