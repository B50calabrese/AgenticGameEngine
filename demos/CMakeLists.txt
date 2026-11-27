# Demos CMakeLists

# Sandbox demo
add_executable(sandbox_demo
    sandbox/main.cpp
)
target_link_libraries(sandbox_demo
    PRIVATE
        engine::engine
)

# Window demo
add_executable(window_demo
    window/main.cpp
)
target_link_libraries(window_demo
    PRIVATE
        engine::engine
)

# Primitives demo
add_executable(primitives_demo
    primitives/main.cpp
)
target_link_libraries(primitives_demo
    PRIVATE
        engine::engine
)

# Breakout demo
add_executable(breakout_demo
    breakout/main.cpp
)
target_link_libraries(breakout_demo
    PRIVATE
        engine::engine
)

# Platformer demo
add_executable(platformer_demo
    platformer/main.cpp
)
target_link_libraries(platformer_demo
    PRIVATE
        engine::engine
)

# Textures demo
add_executable(textures_demo
    textures/main.cpp
)
target_link_libraries(textures_demo
    PRIVATE
        engine::engine
)

# Font demo
add_executable(font_demo
    font_demo/main.cpp
)
target_link_libraries(font_demo
    PRIVATE
        engine::engine
)
