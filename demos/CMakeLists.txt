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
