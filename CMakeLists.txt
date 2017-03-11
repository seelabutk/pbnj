CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(PeanutButterNJelly)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/")
SET(OSPRAY_BUILD_DIR "/export/ahota/ospray/1.1.0/build/"
    CACHE FILEPATH "Path to OSPRay build directory")

SET(USE_NETCDF OFF CACHE BOOL "Enable NetCDF file reading")

FIND_PACKAGE(OSPRay REQUIRED)

#USE_NETCDF adds netcdf includes/libs for data reading
IF(USE_NETCDF)
    # give cmake some hints to finding NetCDF - god knows it needs it
    SET(NETCDF_INCLUDE_DIR "/usr/include"
        CACHE FILEPATH "Path to NetCDF includes")
    SET(NETCDF_LIBRARY "/usr/lib/x86_64-linux-gnu/libnetcdf_c++4.so"
        CACHE FILEPATH "Path to NetCDF library")
    # set this else the findnetcdf won't look in system paths
    SET(NETCDF_USE_DEFAULT_PATHS ON)

    FIND_PACKAGE(NetCDF COMPONENTS CXX)

    IF(NETCDF_FOUND)
        ADD_DEFINITIONS(-DPBNJ_NETCDF)
        INCLUDE_DIRECTORIES(${NETCDF_CXX_INCLUDE_DIRS})
    ELSE()
        MESSAGE(FATAL_ERROR
            "Requested NetCDF but the library could not be found")
    ENDIF()
ENDIF(USE_NETCDF)

INCLUDE_DIRECTORIES(include ${OSPRAY_INCLUDE_DIRS})

SET(PBNJ_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include" ${OSPRAY_INCLUDE_DIRS})
FILE(GLOB SOURCES "src/*.cpp")

ADD_LIBRARY(pbnj SHARED ${SOURCES})

INCLUDE_DIRECTORIES(include "/home/mohammad/repos/lodepng")

ADD_EXECUTABLE(simpleVolumeRender ${SOURCES} "src/test/simpleVolumeRender.cpp")

TARGET_INCLUDE_DIRECTORIES(pbnj PUBLIC ${PBNJ_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(pbnj ${OSPRAY_LIBRARIES})
TARGET_LINK_LIBRARIES(simpleVolumeRender ${OSPRAY_LIBRARIES})

IF(USE_NETCDF AND NETCDF_FOUND)
    #SET(NETCDF_CXX_LIBRARIES "/usr/lib/x86_64-linux-gnu/libnetcdf_c++4.so")
    MESSAGE("NetCDF include dirs: " ${NETCDF_CXX_INCLUDE_DIRS})
    MESSAGE("NetCDF libraries: " ${NETCDF_CXX_LIBRARIES})
    TARGET_INCLUDE_DIRECTORIES(pbnj PUBLIC ${NETCDF_CXX_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(pbnj ${NETCDF_CXX_LIBRARIES})
    TARGET_LINK_LIBRARIES(simpleVolumeRender ${NETCDF_CXX_LIBRARIES})
ENDIF()
