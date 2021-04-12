add_executable(BetaRay BetaRay/main.cpp)

target_compile_options(BetaRay PRIVATE "/std:c++latest")

target_include_directories(BetaRay PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${STB_INCLUDE_DIR}
)

target_link_libraries(BetaRay PRIVATE
    glm::glm
)