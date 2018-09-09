CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(PeanutButterNJelly)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/")

SET(embree_DIR ""
    CACHE FILEPATH "Directory containing an embree-config.cmake file")
SET(ospray_DIR ""
    CACHE FILEPATH "Directory containing an osprayConfig.cmake file")
FIND_PACKAGE(ospray REQUIRED)
FIND_PACKAGE(embree "2.15.0" REQUIRED)

OPTION(USE_NETCDF "Enable NetCDF file reading" ON)
OPTION(BUILD_EXAMPLES "Build example applications" ON)
OPTION(BUILD_DOCUMENTATION "Build documentation with Sphinx" OFF)

SET(PBNJ_LIBS ${EMBREE_LIBRARIES} ${OSPRAY_LIBRARIES})
SET(PBNJ_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include" "${CMAKE_CURRENT_LIST_DIR}/include/pugixml"
    ${OSPRAY_INCLUDE_DIRS} ${EMBREE_INCLUDE_DIRS})

MESSAGE(STATUS "PBNJ LIBS: ${PBNJ_LIBS}")
MESSAGE(STATUS "OSPRAY LIBS: ${OSPRAY_LIBRARIES}")
MESSAGE(STATUS "EMBREE LIBS: ${EMBREE_LIBRARIES}")


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
        SET(PBNJ_INCLUDE_DIRS ${PBNJ_INCLUDE_DIRS}
            ${NETCDF_CXX_INCLUDE_DIRS})
        SET(PBNJ_LIBS ${PBNJ_LIBS} ${NETCDF_CXX_LIBRARIES})
    ELSE()
        MESSAGE(FATAL_ERROR
            "Requested NetCDF but the library could not be found")
    ENDIF()
ENDIF(USE_NETCDF)

FILE(GLOB PBNJ_SOURCES "src/*.cpp" "src/lodepng/*.cpp" "src/pugixml/*.cpp")

ADD_LIBRARY(pbnj SHARED ${PBNJ_SOURCES})
TARGET_INCLUDE_DIRECTORIES(pbnj PUBLIC ${PBNJ_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(pbnj ${PBNJ_LIBS} pthread jpeg)

IF(BUILD_EXAMPLES)
    ADD_EXECUTABLE(simpleVolumeRender ${PBNJ_SOURCES}
        "src/test/simpleVolumeRender.cpp")
    TARGET_LINK_LIBRARIES(simpleVolumeRender ${PBNJ_LIBS} pthread jpeg)
    TARGET_INCLUDE_DIRECTORIES(simpleVolumeRender PUBLIC ${PBNJ_INCLUDE_DIRS})
    ADD_EXECUTABLE(timeSeries ${PBNJ_SOURCES} "src/test/timeSeries.cpp")
    TARGET_LINK_LIBRARIES(timeSeries ${PBNJ_LIBS} pthread jpeg)
    TARGET_INCLUDE_DIRECTORIES(timeSeries PUBLIC ${PBNJ_INCLUDE_DIRS})
    ADD_EXECUTABLE(benchmark ${PBNJ_SOURCES} "src/test/benchmark.cpp")
    TARGET_LINK_LIBRARIES(benchmark ${PBNJ_LIBS} pthread jpeg)
    TARGET_INCLUDE_DIRECTORIES(benchmark PUBLIC ${PBNJ_INCLUDE_DIRS})
    ADD_EXECUTABLE(omni ${PBNJ_SOURCES} "src/test/omni.cpp")
    TARGET_LINK_LIBRARIES(omni ${PBNJ_LIBS} pthread jpeg)
    TARGET_INCLUDE_DIRECTORIES(omni PUBLIC ${PBNJ_INCLUDE_DIRS})
    ADD_EXECUTABLE(particleTest ${PBNJ_SOURCES} "src/test/particleTest.cpp")
    TARGET_LINK_LIBRARIES(particleTest ${PBNJ_LIBS} pthread jpeg)
    TARGET_INCLUDE_DIRECTORIES(particleTest PUBLIC ${PBNJ_INCLUDE_DIRS})
    ADD_EXECUTABLE(streamlinesTest ${PBNJ_SOURCES} "src/test/streamlinesTest.cpp")
    TARGET_LINK_LIBRARIES(streamlinesTest ${PBNJ_LIBS} pthread jpeg)
    TARGET_INCLUDE_DIRECTORIES(streamlinesTest PUBLIC ${PBNJ_INCLUDE_DIRS})
ENDIF(BUILD_EXAMPLES)

IF(BUILD_DOCUMENTATION)
    ADD_SUBDIRECTORY(docs)
ENDIF(BUILD_DOCUMENTATION)

# install rules
INSTALL(TARGETS pbnj LIBRARY DESTINATION lib)
INSTALL(DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/include/" DESTINATION include
    FILES_MATCHING PATTERN "*.h*")
