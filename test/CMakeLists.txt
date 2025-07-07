add_executable(run_tests tests.cpp)

target_link_libraries(run_tests PRIVATE geometria gtest_main)

include(GoogleTest)
gtest_add_tests(TARGET run_tests)
