cmake_minimum_required(VERSION 2.8.3)
project(uri_scripts)

set(CMAKE_CXX_COMPILER_ID_TEST_FLAGS "-std=c++11")
# set(CMAKE_C '-std=c++11')
## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
)



###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  INCLUDE_DIRS 
  LIBRARIES 
  CATKIN_DEPENDS roscpp rospy 
  DEPENDS 
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
# add_library(uri_scripts
# 	src/trajectory.cpp
# 	src/shared_memory.cpp
# 	)
# target_link_libraries(uri_scripts ${catkin_LIBRARIES} ${EIGEN_LIBRARIES})
#message(WARNING ${EIGEN_LIBRARIES})


