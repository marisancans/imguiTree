cmake_minimum_required(VERSION 3.12)
project(imguitest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SOURCES ${SRC_DIR}/main.cpp
            ${SRC_DIR}/imgui/imconfig.h
            ${SRC_DIR}/imgui/imgui.h
            ${SRC_DIR}/imgui/imgui.cpp
            ${SRC_DIR}/imgui/imgui_demo.cpp
            ${SRC_DIR}/imgui/imgui_draw.cpp
            ${SRC_DIR}/imgui/imgui_impl_glfw.h
            ${SRC_DIR}/imgui/imgui_impl_glfw.cpp
            ${SRC_DIR}/imgui/imgui_impl_opengl3.cpp
            ${SRC_DIR}/imgui/imgui_internal.h
            ${SRC_DIR}/imgui/imgui_widgets.cpp
            ${SRC_DIR}/imgui/imstb_rectpack.h
            ${SRC_DIR}/imgui/imstb_textedit.h
            ${SRC_DIR}/imgui/imstb_truetype.h
            ${SRC_DIR}/imgui/imgui_node_graph_test.cpp
            ${SRC_DIR}/imgui/imgui_node_graph_test.h
            ${SRC_DIR}/node.cpp
            ${SRC_DIR}/node.h
            ${SRC_DIR}/game.cpp
            ${SRC_DIR}/game.h
            ${SRC_DIR}/layer.cpp
            ${SRC_DIR}/layer.h src/playerStats.h)


add_executable(${PROJECT_NAME} ${SOURCES})


# glfw
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")
target_link_libraries(${PROJECT_NAME} glfw GLEW GL)