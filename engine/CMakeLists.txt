file(GLOB sources src/*.cpp)
file(GLOB headers include/*.h)

include_directories(include 
    "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/include")
link_directories("C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/lib/x86")

add_definitions("-DD3D9_ENGINE_EXPORTS")


add_library(engine SHARED ${sources} ${headers})
target_link_libraries(engine "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/d3dx9.lib" dxguid.lib d3d9.lib)

source_group("source" FILES ${sources})
source_group("include" FILES ${headers})

add_custom_command(TARGET engine POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/build/engine/$<CONFIGURATION>/engine.dll"      # <--this is in-file
        ${PROJECT_SOURCE_DIR}/build/testgame/$<CONFIGURATION>)                 # <--this is out-file path