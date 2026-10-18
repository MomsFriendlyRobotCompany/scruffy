
add_executable(test
    pass.c
    fail.c
    test.c
)
target_link_libraries(test ${PROJECT_NAME})
scruffy_info(test)