add_executable(core_test
    core_test.cpp
)
target_link_libraries(core_test
    PRIVATE
        engine::engine
        GTest::gtest_main
)

add_executable(window_test
    window_test.cpp
)
target_link_libraries(window_test
    PRIVATE
        engine::engine
        GTest::gtest_main
)

add_executable(renderer_test
    renderer_test.cpp
)
target_link_libraries(renderer_test
    PRIVATE
        engine::engine
        GTest::gtest_main
)

add_executable(math_test
    math_test.cpp
)
target_link_libraries(math_test
    PRIVATE
        engine::engine
        GTest::gtest_main
)

add_executable(input_test
    input_test.cpp
)
target_link_libraries(input_test
    PRIVATE
        engine::engine
        GTest::gtest_main
)

add_executable(texture_test
    texture_test.cpp
)
target_link_libraries(texture_test
    PRIVATE
        engine::engine
        GTest::gtest_main
)

add_executable(font_test
    font_test.cpp
)
target_link_libraries(font_test
    PRIVATE
        engine::engine
        GTest::gtest_main
)

add_executable(ui_test
    ui_test.cpp
)
target_link_libraries(ui_test
    PRIVATE
        engine::engine
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(core_test)
gtest_discover_tests(window_test)
gtest_discover_tests(renderer_test)
gtest_discover_tests(math_test)
gtest_discover_tests(input_test)
gtest_discover_tests(texture_test)
gtest_discover_tests(font_test)
gtest_discover_tests(ui_test)
