cmake_minimum_required( VERSION 3.5 )

project( cib-examples VERSION 1.0.0 LANGUAGES C CXX )

set( CMAKE_POSITION_INDEPENDENT_CODE ON )

add_subdirectory( external )

add_subdirectory( ticker-basic )
