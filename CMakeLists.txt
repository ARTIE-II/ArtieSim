cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)

project(Artie)

set(CMAKE_BUILD_TYPE Debug)

# Debug mode
option(DEBUG_MODE "Compile with debug options" OFF)
if(DEBUG_MODE)
    add_compile_options(-g)
endif()

# profiling mode
option(PROFILING_MODE "Compile with profiling turned on" ON)
if(PROFILING_MODE)
    add_definitions(-DARTIE_PROFILING)
    message(STATUS "Compiling with profiling mode turned on")
endif()

# Multithreading mode
option(MULTITHREADING "Compile with multithreading mode" OFF)
if(MULTITHREADING)
    if(DEFINED ENV{G4MULTITHREADED})
        add_definitions(-DARTIE_MULTITHREADED)
        message(STATUS "Compiling with multithreading enabled.")
    else()
        message(STATUS "Attempt to use multithreading failed - G4MULTITHREADED is not defined! Maybe you didn't compile G4 with multithreading.")
    endif()
endif()

# use the visualization manager
option(USE_VIS "Whether to use the visualization." OFF)
if(USE_VIS)
    add_definitions(-DARTIE_USE_VIS)
    message(STATUS "Compiling with the visualization enabled.")
endif()

# use the ui executive/manager
option(USE_UI "Whether to use the ui interface." OFF)
if(USE_UI)
    add_definitions(-DARTIE_USE_UI)
    message(STATUS "Compiling with the ui enabled.")
endif()

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
message("Geant4 installation directory -- ${Geant4_DIR}")

# Get all header directories
MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.hh)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(header_list)

include_directories(
    ${header_list}
)

# generate sources and headers
file(GLOB_RECURSE sources 
    ${PROJECT_SOURCE_DIR}/src/*.cxx
)
file(GLOB_RECURSE headers 
    ${PROJECT_SOURCE_DIR}/include/*.hh
)

#-----------create programs-----------#
# detector test
add_executable(detector_test 
    src/programs/DetectorTest.cc 
    ${sources} ${headers} ${NEST_SOURCES}
)
target_link_libraries(detector_test ${Geant4_LIBRARIES})

# copy macro and data files
file(GLOB MACRO_FILES
    "macros/*.mac"
)
file(GLOB DATA_FILES
    "data/*.dat"
)
file(GLOB ANALYSIS_FILES
    "analysis/*.py"
)
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${DATA_FILES} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${ANALYSIS_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_custom_target(Artie DEPENDS artie)