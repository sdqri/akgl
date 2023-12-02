cmake_minimum_required(VERSION 3.0)

project(akgl_project C)

# Add executable
add_executable(akgl main.c glad.c shader/shader.c mesh/mesh.c)

# Link libraries
target_link_libraries(akgl PRIVATE glfw dl GL m)
