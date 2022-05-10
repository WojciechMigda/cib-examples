# ./external/cib/CMakeLists.txt -- Traditional CMake

cmake_minimum_required( VERSION 2.8.10 )

project( External.cib )
set( VERSION 1.0 )
set( DESCRIPTION "CIB header-only lib." )

# Provide target for "cib" library.
add_library( cib UNKNOWN
    IMPORTED GLOBAL )

set_target_properties( cib PROPERTIES
    MY_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR} )
