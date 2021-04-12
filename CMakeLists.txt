cmake_minimum_required(VERSION 3.16)

project(BetaRay)

find_package(glm CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

add_subdirectory(src)