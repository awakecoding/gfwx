
set(MODULE_NAME "gfwx-app")
set(MODULE_PREFIX "GFWX_APP")

find_package(OpenCV)

if(OPENCV_FOUND)
	add_definitions("-DWITH_OPENCV")
endif()

find_package(OpenMP)

if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

set(${MODULE_PREFIX}_HEADERS "")

set(${MODULE_PREFIX}_SOURCES
        main.cpp)

add_executable(${MODULE_NAME} ${${MODULE_PREFIX}_SOURCES})

target_link_libraries(${MODULE_NAME} gfwx)

if(OPENCV_FOUND)
	target_link_libraries(${MODULE_NAME} ${OpenCV_LIBS})
endif()

set_target_properties(${MODULE_NAME} PROPERTIES OUTPUT_NAME "gfwx")
set_property(TARGET ${MODULE_NAME} PROPERTY FOLDER "gfwx/app")
