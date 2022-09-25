
add_executable(
    brownian_motion
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rlut/rlut.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/app/app.cpp
)

target_compile_features(
    brownian_motion
    PUBLIC
    cxx_std_20
)

target_include_directories(
    brownian_motion
    PUBLIC
    ${RAYLIB_DIR}/include
    ${SRC_DIR}
)

target_link_libraries(
    brownian_motion
    ${RAYLIB_DIR}/lib/raylib.lib
    winmm.lib
)
