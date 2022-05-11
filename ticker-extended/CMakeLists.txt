project( CIB-examples LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )

set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

include_directories(
    "$<TARGET_PROPERTY:cib,MY_INCLUDE_DIRS>"
    "$<TARGET_PROPERTY:fmt,MY_INCLUDE_DIRS>"
)

add_executable( ticker-extended "src/dummy.cpp" )
target_sources( ticker-extended PRIVATE
    src/main.cpp
)

target_compile_definitions( ticker-extended PUBLIC
    "$<TARGET_PROPERTY:cib,INTERFACE_COMPILE_DEFINITIONS>"
    "$<TARGET_PROPERTY:fmt,INTERFACE_COMPILE_DEFINITIONS>"
)
target_include_directories( ticker-extended PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src )
