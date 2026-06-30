# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)
project(SNN_STDP_Visualizer)

# Enable the CTest framework for the entire project
enable_testing()

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required external libraries
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)
find_package(GTest CONFIG REQUIRED)

# Group all core source files
set(CORE_SOURCES
    src/Net.cpp
    src/Neuron.cpp
    src/Synapse.cpp
    src/NetworkVisualizer.cpp
)

# --- 1. THE MAIN PROGRAM (Visual Simulation) ---
add_executable(SNN_App src/main.cpp ${CORE_SOURCES})
target_include_directories(SNN_App PRIVATE src)
target_link_libraries(SNN_App PRIVATE SFML::Graphics SFML::Window SFML::System)

# --- 2. THE TEST PROGRAM (Mathematical Verification) ---
add_executable(SNN_Tests tests/test_neuron.cpp tests/test_synapse.cpp tests/test_net.cpp ${CORE_SOURCES})
target_include_directories(SNN_Tests PRIVATE src)
# GTest::gtest_main provides the hidden main() function for our tests
target_link_libraries(SNN_Tests PRIVATE SFML::Graphics SFML::Window SFML::System GTest::gtest GTest::gtest_main)

# --- 3. REGISTER TESTS ---
# Include the GoogleTest module and discover tests within the executable
include(GoogleTest)
gtest_discover_tests(SNN_Tests)