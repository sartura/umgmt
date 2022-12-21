# test user data type
set(
    USER_UTEST_LINKER_OPTIONS
    "-Wl,--wrap=strdup"
)
add_executable(
    test_user

    test/test_user.c
)

target_link_libraries(
    test_user

    ${CMOCKA_LIBRARIES}
    ${SYSREPO_LIBRARIES}
    ${LIBYANG_LIBRARIES}
    ${CMAKE_PROJECT_NAME}
)
target_link_options(test_user PRIVATE ${USER_UTEST_LINKER_OPTIONS})
add_test(NAME test_user COMMAND test_user)

# test group data type
set(
    GROUP_UTEST_LINKER_OPTIONS
    "-Wl,--wrap=strdup"
    "-Wl,--wrap=malloc"
)

add_executable(
    test_group

    test/test_group.c
)

target_link_libraries(
    test_group

    ${CMOCKA_LIBRARIES}
    ${SYSREPO_LIBRARIES}
    ${LIBYANG_LIBRARIES}
    ${CMAKE_PROJECT_NAME}
)
target_link_options(test_group PRIVATE ${GROUP_UTEST_LINKER_OPTIONS})
add_test(NAME test_group COMMAND test_group)

# test database data type
add_executable(
    test_db

    test/test_db.c
)

target_link_libraries(
    test_db

    ${CMOCKA_LIBRARIES}
    ${SYSREPO_LIBRARIES}
    ${LIBYANG_LIBRARIES}
    ${CMAKE_PROJECT_NAME}
)
add_test(NAME test_db COMMAND test_db)