#----------------------------------------------------------------------------
# Setup the project
#----------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(CAENHV_CTRL)

#----------------------------------------------------------------------------
# REQUIRE GTK3
#----------------------------------------------------------------------------

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
message(STATUS "----- GTK3_INCLUDE_DIRS: ${GTK3_INCLUDE_DIRS}")
message(STATUS "----- GKT3_LIBRARIES: ${GTK3_LIBRARIES}")
message(STATUS "----- GTK3_LINK_LIBRARIES: ${GTK3_LINK_LIBRARIES}")
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
list(APPEND FC_DEP_LIBS ${GTK3_LIBRARIES})

#----------------------------------------------------------------------------
# Copying GLADE XML GUI
#----------------------------------------------------------------------------

set(
  CAENHV_CTRL
  glade/gui.glade
  docs/icon.png
)

foreach(_script ${CAENHV_CTRL})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(--std=gnu99)
endif()


include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(caenhvctrl caenhvctrl.c ${sources} ${headers})
target_link_libraries(caenhvctrl -rdynamic)
target_link_libraries (caenhvctrl ${FC_DEP_LIBS})


option(DEBUG "DEBUG" ON)
if(DEBUG)
  message("-- Building with console debug.")
  add_definitions(-DDEBUG)
else()
  message("-- Building without console debug.")
endif()
#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS caenhvctrl DESTINATION bin)
