cmake_minimum_required (VERSION 2.8.12.1)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)
project (gamma-view)

#generate list of files in folder
file (GLOB MAIN_SOURCES 
	"main.h"
	"main.cpp")
file (GLOB REST_SOURCES 
	"*.h"
	"*.cpp")
# file (GLOB GUI_SOURCES 
	# "gui/*.h"
	# "gui/*.cpp")
# file (GLOB MODEL_SOURCES 
	# "model/*.h"
	# "model/*.cpp")
file (GLOB RESOURCES 
	"gamma-view.ico"
	"gamma-view.rc")
	

	
source_group ("" FILES ${MAIN_SOURCES})
source_group ("Rest" FILES ${REST_SOURCES})
# source_group ("GUI" FILES ${GUI_SOURCES})
# source_group ("Model" FILES ${MODEL_SOURCES})
source_group ("Resources" FILES ${RESOURCES})

add_definitions (-D_CRT_SECURE_NO_WARNINGS=1)
	
include_directories (${CMAKE_CURRENT_SOURCE_DIR})

# mswud / mswu
set (wxWidgets_CONFIGURATION mswud)
set (CMAKE_BUILD_TYPE Debug)
find_package (wxWidgets REQUIRED adv core base)
include (${wxWidgets_USE_FILE})

include_directories ("C:\\Cypress\\Cypress Suite USB 3.4.7\\CyAPI\\inc")
include_directories ("C:\\libusb-win32-bin-1.2.6.0\\include")

add_executable (gamma-view WIN32  
	${MAIN_SOURCES}
	${REST_SOURCES}
	# ${GUI_SOURCES} 
	# ${MODEL_SOURCES} 
	${RESOURCES})
	
target_link_libraries (gamma-view ${wxWidgets_LIBRARIES})
target_link_libraries (gamma-view "C:\\Cypress\\Cypress Suite USB 3.4.7\\CyAPI\\lib\\x86\\CyAPI.lib")
target_link_libraries (gamma-view "C:\\libusb-win32-bin-1.2.6.0\\lib\\msvc\\libusb.lib")
target_link_libraries (gamma-view "C:\\libusb-win32-bin-1.2.6.0\\lib\\gcc\\libusb.a")

if (VERBOSE)
	get_cmake_property(_variableNames VARIABLES)
	foreach (_variableName ${_variableNames})
		message(STATUS "${_variableName}=${${_variableName}}")
	endforeach()
endif()
