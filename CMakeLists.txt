# Copyright (c) 2011-2015:  G-CSC, Goethe University Frankfurt
# Author: Sebastian Reiter
# 
# This file is part of UG4.
# 
# UG4 is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License version 3 (as published by the
# Free Software Foundation) with the following additional attribution
# requirements (according to LGPL/GPL v3 §7):
# 
# (1) The following notice must be displayed in the Appropriate Legal Notices
# of covered and combined works: "Based on UG4 (www.ug4.org/license)".
# 
# (2) The following notice must be displayed at a prominent place in the
# terminal output of covered works: "Based on UG4 (www.ug4.org/license)".
# 
# (3) The following bibliography is recommended for citation and must be
# preserved in all covered files:
# "Reiter, S., Vogel, A., Heppner, I., Rupp, M., and Wittum, G. A massively
#   parallel geometric multigrid solver on hierarchically distributed grids.
#   Computing and visualization in science 16, 4 (2013), 151-164"
# "Vogel, A., Reiter, S., Rupp, M., Nägel, A., and Wittum, G. UG4 -- a novel
#   flexible software system for simulating pde based models on high performance
#   computers. Computing and visualization in science 16, 4 (2013), 165-179"
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.

################################################################################
# SAMPLE
#
# This script shows how easy it is to add a build script for your plugins.
# Simply specify the plugins name and a list of your source files.
################################################################################

# Name of your plugin and sources.
set(pluginName	Jupyter)
set(SOURCES		jupyter_plugin.cpp)


################################################################################
# The code below doesn't have to be changed (usually)
################################################################################
cmake_minimum_required(VERSION 3.1)

project(UG_PLUGIN_${pluginName})

# WE require C++ 17
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")


# Various Xeus libs
find_package(xwidgets REQUIRED) 
find_package(xeus REQUIRED)
find_package(xtl REQUIRED)

message(STATUS "Jupyter-Plugin: xwidgets_FOUND=${xwidgets_FOUND} ${xwidgets_INCLUDE_DIRS}")
message(STATUS "Jupyter-Plugin: xeus_FOUND=${xeus_FOUND} ${xeus_INCLUDE_DIRS} ")
message(STATUS "Jupyter-Plugin: xtl_FOUND=${xtl_FOUND} ${xtl_INCLUDE_DIRS}")

# Find libraries
if(NOT DEFINED ${xeus_LIBRARY})
	find_library(xeus_LIBRARY NAMES xeus PATHS "${xtl_INCLUDE_DIRS}/../lib")
endif() 

if(NOT DEFINED ${xwidgets_LIBRARY})
	find_library(xwidgets_LIBRARY NAMES xtl PATHS "${xtl_INCLUDE_DIRS}/../lib")
endif() 

message(STATUS "Jupyter-Plugin: using  ${xeus_LIBRARY} ${xwidgets_LIBRARY}")   




# We also need  'zeromq' and 'sodium'
## load in pkg-config support
message(STATUS "Jupyter-Plugin: Checking pkg-config ${CMAKE_PREFIX_PATH}")
find_package(PkgConfig)
pkg_check_modules(PC_ZeroMQ zmq) ## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_Sodium sodium) ## use pkg-config to get hints for sodium locations

## use the hint from about to find the location of libzmq, sodium
if(NOT DEFINED ${PC_ZeroMQ_LIBRARY_DIRS})
	set (PC_ZeroMQ_LIBRARY_DIRS "${xtl_INCLUDE_DIRS}/../lib")
	message(WARNING "Jupyter-Plugin: $PC_ZeroMQ_LIBRARY_DIRS not defined.")
endif()	

find_library(ZeroMQ_LIBRARY NAMES zmq PATHS ${PC_ZeroMQ_LIBRARY_DIRS})
find_library(sodium_LIBRARY NAMES sodium PATHS ${PC_ZeroMQ_LIBRARY_DIRS})


# include the definitions and dependencies for ug-plugins.
include(${UG_ROOT_CMAKE_PATH}/ug_plugin_includes.cmake)


# Expand include path
include_directories(AFTER ${xtl_INCLUDE_DIRS}
	# ${xeus_INCLUDE_DIRS}
	# ${xwidgets_INCLUDE_DIRS}
	)
	

if(buildEmbeddedPlugins)
	# add the sources to ug4's sources
	EXPORTSOURCES(${CMAKE_CURRENT_SOURCE_DIR} ${SOURCES})
else(buildEmbeddedPlugins)
	# create a shared library from the sources and link it against ug4.
	add_library(${pluginName} SHARED ${SOURCES})
	target_link_libraries (${pluginName} PUBLIC ug4 
	 	${xeus_LIBRARY} ${xwidgets_LIBRARY}
	 	${sodium_LIBRARY} ${ZeroMQ_LIBRARY}
	 	)
endif(buildEmbeddedPlugins)
