file(GLOB sources src/*.cpp)

include_directories(../engine/include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/include")
link_directories("C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/lib")

add_executable(testgame ${sources})
target_link_libraries(testgame engine)
