# CMakeList.txt : CMake project for LearnOpenGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

# assimp-5.2.4 中 构建 tools 目录下的工具需要以来dx9, 在window10的sdk10中dx9已经被移除
# 会造成cmake 配置错误
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
add_subdirectory(glad-202204)
add_subdirectory(glfw-3.3.6)
add_subdirectory(glm-0.9.9.8)
add_subdirectory(assimp-5.2.4)
add_subdirectory(LearnOpenGL)
# TODO: Add tests and install targets if needed.
