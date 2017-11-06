
set(MODULE_NAME "gfwx-app")
set(MODULE_PREFIX "GFWX_APP")

# zlib

find_path(ZLIB_INCLUDE_DIR
	NAMES zlib.h
	PATH_SUFFIXES include)

find_library(ZLIB_LIBRARY
	NAMES libz.a zlib.lib
	PATH_SUFFIXES lib)

if(ZLIB_INCLUDE_DIR AND ZLIB_LIBRARY)
	set(ZLIB_FOUND TRUE)
	set(ZLIB_LIBRARIES ${ZLIB_LIBRARY})

	add_library(zlib STATIC IMPORTED)
	set_property(TARGET zlib PROPERTY IMPORTED_LOCATION ${ZLIB_LIBRARY})
endif()

if(ZLIB_FOUND)
	add_definitions("-DWITH_ZLIB")
	message(STATUS "Found zlib: ${ZLIB_LIBRARIES}")
else()
	message(WARNING "zlib was not found")
endif()

# libpng

if(ZLIB_FOUND)
	find_path(PNG_INCLUDE_DIR
		NAMES libpng16/png.h
		PATH_SUFFIXES include)

	find_library(PNG_LIBRARY
		NAMES libpng16${CMAKE_STATIC_LIBRARY_SUFFIX}
		PATH_SUFFIXES lib)

	if(PNG_INCLUDE_DIR AND PNG_LIBRARY)
		set(PNG_FOUND TRUE)
		set(PNG_LIBRARIES ${PNG_LIBRARY})

		add_library(png STATIC IMPORTED)
		set_property(TARGET png PROPERTY IMPORTED_LOCATION ${PNG_LIBRARY})
		set_property(TARGET png PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES zlib)
	endif()

	if(PNG_FOUND)
		add_definitions("-DWITH_PNG")
		message(STATUS "Found png: ${PNG_LIBRARIES}")
	else()
		message(WARNING "png was not found")
	endif()
endif()

if(PNG_FOUND)
	include_directories(${PNG_INCLUDE_DIR})
endif()

find_package(OpenMP)

if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

set(${MODULE_PREFIX}_HEADERS
	gfwx_color.h
	gfwx_image.h
	gfwx_file.h
	gfwx_time.h
	gfwx_app.h)

set(${MODULE_PREFIX}_SOURCES
	gfwx_color.c
	gfwx_image.c
	gfwx_file.c
	gfwx_time.c
	gfwx_app.cpp)

add_executable(${MODULE_NAME} ${${MODULE_PREFIX}_SOURCES} ${${MODULE_PREFIX}_HEADERS})

set(${MODULE_PREFIX}_LIBS gfwx)

if(PNG_FOUND)
	list(APPEND ${MODULE_PREFIX}_LIBS png)
endif()

target_link_libraries(${MODULE_NAME} ${${MODULE_PREFIX}_LIBS})

set_target_properties(${MODULE_NAME} PROPERTIES OUTPUT_NAME "gfwx")
set_property(TARGET ${MODULE_NAME} PROPERTY FOLDER "gfwx/app")
