add_executable(sandbox_demo
    sandbox/main.cpp
)
target_link_libraries(sandbox_demo
    PRIVATE
        engine::engine
)

add_executable(window_demo
    window/main.cpp
)
target_link_libraries(window_demo
    PRIVATE
        engine::engine
)

add_executable(primitives_demo
    primitives/main.cpp
)
target_link_libraries(primitives_demo
    PRIVATE
        engine::engine
)

add_executable(breakout_demo
    breakout/main.cpp
)
target_link_libraries(breakout_demo
    PRIVATE
        engine::engine
)

add_executable(platformer_demo
    platformer/main.cpp
)
target_link_libraries(platformer_demo
    PRIVATE
        engine::engine
)
