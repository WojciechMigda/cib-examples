project( CIB-examples LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )

set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

include_directories(
    "$<TARGET_PROPERTY:cib,MY_INCLUDE_DIRS>"
)

add_executable( app "src/dummy.cpp" )
target_sources( app PRIVATE
    src/main.cpp
)

target_compile_definitions( app PUBLIC
    "$<TARGET_PROPERTY:cib,INTERFACE_COMPILE_DEFINITIONS>"
)
target_include_directories( app PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src )
