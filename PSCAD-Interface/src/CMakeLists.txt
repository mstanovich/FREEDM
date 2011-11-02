# include project source directories
include_directories ("${SIMULATION_SERVER_SOURCE_DIR}/include")
include_directories ("${SIMULATION_SERVER_SOURCE_DIR}/src")

# list the project source files
set (
    SOURCE_FILES
    CDeviceKey.cpp
    CTableStructure.cpp
    CDeviceTable.cpp
    CLineServer.cpp
    CLineClient.cpp
)

# set C++ compiler flags
set (CMAKE_CXX_FLAGS "-std=c++98 -W -Wall -pedantic")

# name the executable
add_library (SIMSERV ${SOURCE_FILES})
add_executable (PosixServer PosixMain.cpp)

# link the executable to its dependencies
target_link_libraries (PosixServer ${Boost_THREAD_LIBRARY})
target_link_libraries (PosixServer ${Boost_SYSTEM_LIBRARY})
target_link_libraries (PosixServer SIMSERV)
