# cmake specification for simulation server
cmake_minimum_required (VERSION 2.6)
project (SIMULATION_SERVER)

# find the required boost libraries
find_package (Boost REQUIRED COMPONENTS system thread)

if (Boost_FOUND)
    # add boost libraries to the project
    include_directories (${Boost_INCLUDE_DIRS})
    link_directories (${Boost_LIBRARY_DIRS})
    
    # goto src/CMakeLists.txt
    add_subdirectory (src)
endif (Boost_FOUND)
