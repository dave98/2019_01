# Install script for directory: C:/Users/ASUS/Desktop/FANN-2.2.0-Source/src/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/FANN")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "C:/Users/ASUS/Desktop/FANN-2.2.0-Source/src/include/fann.h"
    "C:/Users/ASUS/Desktop/FANN-2.2.0-Source/src/include/doublefann.h"
    "C:/Users/ASUS/Desktop/FANN-2.2.0-Source/src/include/fann_internal.h"
    "C:/Users/ASUS/Desktop/FANN-2.2.0-Source/src/include/floatfann.h"
    "C:/Users/ASUS/Desktop/FANN-2.2.0-Source/src/include/fann_data.h"
    "C:/Users/ASUS/Desktop/FANN-2.2.0-Source/src/include/fixedfann.h"
    "C:/Users/ASUS/Desktop/FANN-2.2.0-Source/src/include/compat_time.h"
    "C:/Users/ASUS/Desktop/FANN-2.2.0-Source/src/include/fann_activation.h"
    "C:/Users/ASUS/Desktop/FANN-2.2.0-Source/src/include/fann_cascade.h"
    "C:/Users/ASUS/Desktop/FANN-2.2.0-Source/src/include/fann_error.h"
    "C:/Users/ASUS/Desktop/FANN-2.2.0-Source/src/include/fann_train.h"
    "C:/Users/ASUS/Desktop/FANN-2.2.0-Source/src/include/fann_io.h"
    "C:/Users/ASUS/Desktop/FANN-2.2.0-Source/src/include/fann_cpp.h"
    )
endif()

