# Copyright (C) 1995-2019, Rene Brun and Fons Rademakers.
# All rights reserved.
#
# For the licensing terms see $ROOTSYS/LICENSE.
# For the list of contributors see $ROOTSYS/README/CREDITS.

###########################################################
# CMakeLists.txt file for building JupyROOT
###########################################################

set(py_sources
  JupyROOT/__init__.py
  JupyROOT/helpers/__init__.py
  JupyROOT/helpers/cppcompleter.py
  JupyROOT/helpers/handlers.py
  JupyROOT/helpers/utils.py
  JupyROOT/html/__init__.py
  JupyROOT/html/cpphighlighter.py
  JupyROOT/kernel/__init__.py
  JupyROOT/kernel/rootkernel.py
  JupyROOT/kernel/utils.py
  JupyROOT/kernel/magics/__init__.py
  JupyROOT/kernel/magics/cppmagic.py
  JupyROOT/kernel/magics/jsrootmagic.py
  JupyROOT/magics/__init__.py
  JupyROOT/magics/cppmagic.py
  JupyROOT/magics/jsrootmagic.py
)

set(JupyROOTPySrcDir python/JupyROOT)
file(COPY ${JupyROOTPySrcDir} DESTINATION ${localruntimedir})

set(libname JupyROOT${PYTHON_UNDER_VERSION_STRING})

# libJupyROOT uses ROOT headers from source dirs and depends on Core
add_library(${libname} SHARED src/IOHandler.cxx)
# Set the suffix to '.so' and the prefix to 'lib'
set_target_properties(${libname} PROPERTIES  ${ROOT_LIBRARY_PROPERTIES})
if(MSVC)
  set_target_properties(${libname} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
  set_target_properties(${libname} PROPERTIES SUFFIX ".pyd")
  target_link_libraries(${libname} PUBLIC Core ${PYTHON_LIBRARIES})
elseif(APPLE)
  target_link_libraries(${libname} PUBLIC -Wl,-bind_at_load -Wl,-w -Wl,-undefined -Wl,dynamic_lookup Core)
else()
  target_link_libraries(${libname} PUBLIC -Wl,--unresolved-symbols=ignore-all Core)
endif()

target_include_directories(${libname} SYSTEM PRIVATE ${PYTHON_INCLUDE_DIRS})

# Compile .py files
foreach(py_source ${py_sources})
  install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} -m py_compile ${localruntimedir}/${py_source})")
  install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} -O -m py_compile ${localruntimedir}/${py_source})")
endforeach()

# Install library
install(TARGETS ${libname} EXPORT ${CMAKE_PROJECT_NAME}Exports
                            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT libraries
                            LIBRARY DESTINATION ${CMAKE_INSTALL_PYTHONDIR} COMPONENT libraries
                            ARCHIVE DESTINATION ${CMAKE_INSTALL_PYTHONDIR} COMPONENT libraries)

# Install Python sources and bytecode
install(DIRECTORY ${localruntimedir}/JupyROOT
        DESTINATION ${CMAKE_INSTALL_PYTHONDIR}
        COMPONENT libraries)
