cmake_minimum_required(VERSION 3.10)
project(MonteCarloPi)

set(CMAKE_CXX_STANDARD 17)  # C++17

# ausführbares Ziel "MonteCarloPi"
add_executable(MonteCarloPi main.cpp
)

# OpenMP
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    target_link_libraries(MonteCarloPi PUBLIC OpenMP::OpenMP_CXX)
    target_compile_options(MonteCarloPi PUBLIC -fopenmp)           # OpenMP-Optionen
endif()
