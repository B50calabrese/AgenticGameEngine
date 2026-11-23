add_executable(sandbox_demo
    sandbox/main.cpp
)

# Demonstrates how downstream consumer projects link to the engine submodule
target_link_libraries(sandbox_demo
    PRIVATE
        engine::engine
)
