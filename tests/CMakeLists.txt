add_executable(core_test
    core_test.cpp
)

target_link_libraries(core_test
    PRIVATE
        engine::engine
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(core_test)
