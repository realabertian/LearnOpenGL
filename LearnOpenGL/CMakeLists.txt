# CMakeList.txt : CMake project for LearnOpenGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("LearnOpenGL")

file(GLOB LearnOpenGL_SRC
     "src/*.h"
     "src/*.cpp"
     "src/*.c"
)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
# Add source to this project's executable.
add_executable(LearnOpenGL ${LearnOpenGL_SRC})
target_link_libraries(LearnOpenGL PRIVATE glad)
target_link_libraries(LearnOpenGL PRIVATE glfw)
target_link_libraries(LearnOpenGL PRIVATE glm::glm)
target_link_libraries(LearnOpenGL PRIVATE assimp::assimp)

# TODO: Add tests and install targets if needed.
