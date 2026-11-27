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

# UI demo
add_executable(ui_demo
    ui_demo/main.cpp
)
target_link_libraries(ui_demo
    PRIVATE
        engine::engine
)

# Lua demo
add_executable(lua_demo
    lua_demo/main.cpp
)
target_link_libraries(lua_demo
    PRIVATE
        engine::engine
)

# Scene & Tweening demo
add_executable(scene_demo
    scene_demo/main.cpp
)
target_link_libraries(scene_demo
    PRIVATE
        engine::engine
)

# ECS demo
add_executable(ecs_demo
    ecs_demo/main.cpp
)
target_link_libraries(ecs_demo
    PRIVATE
        engine::engine
)
