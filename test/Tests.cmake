# test user data type
set(
    USER_UTEST_LINKER_OPTIONS
    "-Wl,--wrap=strdup"
    "-Wl,--wrap=malloc"
)
add_executable(
    test_user

    test/test_user.c
    test/common.c
)

target_link_libraries(
    test_user

    ${CMOCKA_LIBRARIES}
    ${CMAKE_PROJECT_NAME}
)
target_link_options(test_user PRIVATE ${USER_UTEST_LINKER_OPTIONS})
add_test(NAME test_user COMMAND test_user)

# test dyn buffer data type
set(
    DYN_BUFFER_UTEST_LINKER_OPTIONS
    "-Wl,--wrap=strdup"
    "-Wl,--wrap=malloc"
)
add_executable(
    test_dyn_buffer

    test/test_dyn_buffer.c
    test/common.c
)

target_link_libraries(
    test_dyn_buffer

    PRIVATE ${CMOCKA_LIBRARIES}
    PRIVATE umgmt
)
target_link_options(test_dyn_buffer PRIVATE ${DYN_BUFFER_UTEST_LINKER_OPTIONS})
add_test(NAME test_dyn_buffer COMMAND test_dyn_buffer)

# test group data type
set(
    GROUP_UTEST_LINKER_OPTIONS
    "-Wl,--wrap=strdup"
    "-Wl,--wrap=malloc"
)

add_executable(
    test_group

    test/test_group.c
    test/common.c
)

target_link_libraries(
    test_group

    ${CMOCKA_LIBRARIES}
    ${CMAKE_PROJECT_NAME}
)
target_link_options(test_group PRIVATE ${GROUP_UTEST_LINKER_OPTIONS})
add_test(NAME test_group COMMAND test_group)

# test database data type
set(
    DB_UTEST_LINKER_OPTIONS
    "-Wl,--wrap=strdup"
    "-Wl,--wrap=malloc"
)
add_executable(
    test_db

    test/test_db.c
    test/common.c
)

target_link_libraries(
    test_db

    ${CMOCKA_LIBRARIES}
    ${CMAKE_PROJECT_NAME}
)
target_link_options(test_db PRIVATE ${GROUP_UTEST_LINKER_OPTIONS})
add_test(NAME test_db COMMAND test_db)
