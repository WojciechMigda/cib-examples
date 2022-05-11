# ./external/fmt/CMakeLists.txt -- Traditional CMake

cmake_minimum_required( VERSION 2.8.10 )

project( External.fmt )
set( VERSION 1.0 )
set( DESCRIPTION "fmt header-only lib." )

# Provide target for "fmt" library.
add_library( fmt UNKNOWN
    IMPORTED GLOBAL )
# Store include search-path containing headers
# of "fmt".
set_target_properties( fmt PROPERTIES
    MY_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include )

set_property( TARGET fmt APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS FMT_HEADER_ONLY=1 )
